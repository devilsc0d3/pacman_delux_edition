#include "lobbyserver.h"
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>




LobbyServer::LobbyServer(quint16 port, QObject *parent)
    : QObject(parent),
      m_server(new QWebSocketServer(QStringLiteral("Lobby Server"),
                                    QWebSocketServer::NonSecureMode, this))
{
    if (m_server->listen(QHostAddress::Any, port)) {
        connect(m_server, &QWebSocketServer::newConnection,
                this, &LobbyServer::onNewConnection);
        qDebug() << "Server listening on port" << port;
    } else {
        qDebug() << "Failed to start server on port" << port;
    }
}

LobbyServer::~LobbyServer()
{
    m_server->close();
    for (auto &player : m_players)
        player.socket->deleteLater();
}

void LobbyServer::onNewConnection()
{
    if (m_players.size() >= MAX_PLAYERS) {
        QWebSocket *rejected = m_server->nextPendingConnection();
        rejected->sendTextMessage("Lobby full");
        rejected->close();
        rejected->deleteLater();
        qDebug() << "Connection refused: lobby is full.";
        return;
    }

    QWebSocket *socket = m_server->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived,
            this, &LobbyServer::processMessage);
    connect(socket, &QWebSocket::disconnected,
            this, &LobbyServer::socketDisconnected);

    // Temporairement, nom inconnu
    Player p;
    p.name = "Unnamed";
    p.socket = socket;

    m_players.append(p);
    qDebug() << "New player connected. Waiting for name...";

    broadcastLobbyStatus();
}

void LobbyServer::processMessage(const QString &message)
{
    QWebSocket *senderSocket = qobject_cast<QWebSocket *>(sender());

    // Suppose que le client envoie un JSON comme : { "type": "join", "name": "PacMan42" }
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) return;
    QJsonObject obj = doc.object();

    if (obj["type"] == "join" && obj.contains("name")) {
        for (Player &p : m_players) {
            if (p.socket == senderSocket) {
                p.name = obj["name"].toString();
                qDebug() << "Player set name:" << p.name;
                broadcastLobbyStatus();
                return;
            }
        }
    }
}

void LobbyServer::socketDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());

    for (int i = 0; i < m_players.size(); ++i) {
        if (m_players[i].socket == client) {
            qDebug() << "Player disconnected:" << m_players[i].name;
            m_players.removeAt(i);
            break;
        }
    }

    client->deleteLater();
    broadcastLobbyStatus();
}

void LobbyServer::broadcastLobbyStatus()
{
    QJsonArray playerArray;
    for (const Player &p : m_players) {
        playerArray.append(p.name);
    }

    QJsonObject lobbyInfo;
    lobbyInfo["type"] = "lobby_update";
    lobbyInfo["players"] = playerArray;

    QJsonDocument doc(lobbyInfo);
    QString message = doc.toJson(QJsonDocument::Compact);

    for (const Player &p : m_players) {
        p.socket->sendTextMessage(message);
    }
}
