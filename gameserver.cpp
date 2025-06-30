// gameserver.cpp
#include "gameserver.h"
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>

GameServer::GameServer(QObject *parent)
    : QTcpServer{parent}, m_gameScene(nullptr), m_broadcastTimer(nullptr), m_nextPlayerId(0)
{
    m_gameScene = new GameScene(this);
    m_broadcastTimer = new QTimer(this);
    connect(m_broadcastTimer, &QTimer::timeout, this, &GameServer::updateAndBroadcastState);
    connect(m_gameScene, &GameScene::levelWasCleared, this, &GameServer::onLevelCleared);
    connect(m_gameScene, &GameScene::pelletWasEatenAt, this, &GameServer::onBroadcastPelletEaten);
    connect(m_gameScene, &GameScene::allPacmansDied, this, &GameServer::onAllPacmansDied);
}

void GameServer::startServer(quint16 port)
{
    if (listen(QHostAddress::Any, port)) {
        qDebug() << "[Serveur] Serveur démarré et à l'écoute sur le port" << port;
        // On démarre la logique de jeu et le broadcast immédiatement pour les tests
        m_gameScene->startGame();
        m_broadcastTimer->start(1000 / 30);
    } else {
        qDebug() << "[Serveur] Erreur: Impossible de démarrer le serveur:" << errorString();
    }
}

void GameServer::incomingConnection(qintptr socketDescriptor)
{
    if (m_clients.size() >= 4) {
        qDebug() << "[Serveur] Tentative de connexion refusée: partie pleine.";
        QTcpSocket socket;
        socket.setSocketDescriptor(socketDescriptor);
        socket.disconnectFromHost();
        return;
    }
    qDebug() << "[Serveur] Nouvelle connexion, assignation du joueur ID:" << m_nextPlayerId;
    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    ClientInfo newClient;
    newClient.socket = clientSocket;
    newClient.playerId = m_nextPlayerId;
    m_clients.append(newClient);

    RoleAssignment role;
    role.playerId = m_nextPlayerId;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << static_cast<quint8>(Msg_RoleAssignment) << role;
    clientSocket->write(block);

    m_nextPlayerId++;

    connect(clientSocket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "[Serveur] Client" << newClient.playerId << "déconnecté.";
        for (int i = 0; i < m_clients.size(); ++i) {
            if (m_clients[i].socket == clientSocket) {
                m_clients.removeAt(i);
                break;
            }
        }
        clientSocket->deleteLater();
    });

    connect(clientSocket, &QTcpSocket::readyRead, this, [=]() {
        handleClientData(clientSocket);
    });

    // La condition de démarrage à 4 joueurs est retirée pour l'instant.
}

void GameServer::updateAndBroadcastState()
{
    for (const auto& clientInfo : m_clients) {
        sendStateToClient(clientInfo.socket);
    }
}

void GameServer::sendStateToClient(QTcpSocket *client)
{
    if (!client || client->state() != QAbstractSocket::ConnectedState) return;
    GameState currentState;
    currentState.pacmanStates = m_gameScene->getPacmanStates();
    currentState.ghostStates = m_gameScene->getGhostStates();
    currentState.score = m_gameScene->getScore();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << static_cast<quint8>(Msg_GameState) << currentState;
    client->write(block);
}

void GameServer::handleClientData(QTcpSocket *client)
{
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_6_0);
    while (!in.atEnd()) {
        quint8 messageType;
        in >> messageType;
        if (static_cast<MessageType>(messageType) == Msg_PlayerInput) {
            PlayerInput input;
            in >> input;
            m_gameScene->setPacmanDirection(input.playerId, input.direction);
        } else {
            qWarning() << "[Serveur] Type de message inconnu reçu:" << messageType;
            client->readAll();
            return;
        }
    }
}

void GameServer::onLevelCleared()
{
    qDebug() << "[Serveur] Niveau terminé. Notification aux clients et pause.";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << static_cast<quint8>(Msg_LevelCleared);
    for (const auto& clientInfo : m_clients) {
        clientInfo.socket->write(block);
    }
    m_gameScene->getGameTimer()->stop();
    m_broadcastTimer->stop();
    QTimer::singleShot(3000, this, [=](){
        qDebug() << "[Serveur] Rechargement du niveau.";
        m_gameScene->reloadLevel();
        m_gameScene->getGameTimer()->start(1000/60);
        m_broadcastTimer->start(1000/30);
    });
}

void GameServer::onBroadcastPelletEaten(const QPoint &gridPos)
{
    PelletEatenEvent event;
    event.gridPosition = gridPos;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << static_cast<quint8>(Msg_PelletEaten) << event;
    for (const auto& clientInfo : m_clients) {
        clientInfo.socket->write(block);
    }
}

void GameServer::onAllPacmansDied()
{
    qDebug() << "[Serveur] Tous les Pac-Man sont morts. GAME OVER.";
    m_gameScene->gameOver();
    m_gameScene->getGameTimer()->stop();
    m_broadcastTimer->stop();
}
