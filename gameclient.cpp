// gameclient.cpp
#include "gameclient.h"
#include <QDebug>
#include <QDataStream>
#include "networkprotocol.h"

GameClient::GameClient(QObject *parent)
    : QObject{parent}, m_myPlayerId(-1)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &GameClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &GameClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &GameClient::onReadyRead);
}

void GameClient::connectToServer(const QString &hostAddress, quint16 port)
{
    qDebug() << "[Client] Tentative de connexion au serveur" << hostAddress << ":" << port;
    m_socket->connectToHost(hostAddress, port);
}

void GameClient::sendPlayerInput(const QPoint &direction)
{
    if (m_myPlayerId == -1 || !m_socket || m_socket->state() != QAbstractSocket::ConnectedState) return;
    PlayerInput input;
    input.playerId = m_myPlayerId;
    input.direction = direction;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << static_cast<quint8>(Msg_PlayerInput) << input;
    m_socket->write(block);
}

void GameClient::onConnected()
{
    qDebug() << "[Client] Connecté au serveur !";
    emit connected();
}

void GameClient::onDisconnected()
{
    qDebug() << "[Client] Déconnecté du serveur.";
    emit disconnected();
}

void GameClient::onReadyRead()
{
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_6_0);

    while (!in.atEnd()) {
        if(in.status() != QDataStream::Ok) {
            qWarning() << "[Client] Erreur de lecture du flux réseau.";
            return;
        }

        quint8 messageType;
        in >> messageType;

        if (static_cast<MessageType>(messageType) == Msg_RoleAssignment) {
            RoleAssignment role;
            in >> role;
            m_myPlayerId = role.playerId;
            qDebug() << "[Client] Mon rôle m'a été assigné. Je suis le joueur" << m_myPlayerId;
        }
        else if (static_cast<MessageType>(messageType) == Msg_GameState) {
            GameState state;
            in >> state;
            emit gameStateReceived(state);
        }
        else if (static_cast<MessageType>(messageType) == Msg_LevelCleared) {
            qDebug() << "[Client] Reçu message de fin de niveau.";
            emit levelClearedOnClient();
        }
        else if (static_cast<MessageType>(messageType) == Msg_PelletEaten) {
            PelletEatenEvent event;
            in >> event;
            emit pelletRemoved(event.gridPosition);
        }
        else if (static_cast<MessageType>(messageType) == Msg_ResetLevel) {
            qDebug() << "[Client] Reçu message de réinitialisation de niveau.";
            emit resetLevelOnClient();
        }
        else {
            qWarning() << "[Client] Type de message inconnu ou corrompu reçu:" << messageType;
            m_socket->readAll();
            return;
        }
    }
}
