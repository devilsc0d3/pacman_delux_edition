#include "lobbyserver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

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
        qDebug() << "Failed to start server.";
    }
}

LobbyServer::~LobbyServer()
{
    m_server->close();
    for (auto client : m_clientInfo.keys()) {
        client->deleteLater();
    }
}

void LobbyServer::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived, this, &LobbyServer::processMessage);
    connect(socket, &QWebSocket::disconnected, this, &LobbyServer::socketDisconnected);

    qDebug() << "Client connected.";
    broadcastLobbyList();
}

void LobbyServer::processMessage(const QString &message)
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    QString type = obj["type"].toString();

    if (type == "create") {
        QString lobbyId = obj["lobby"].toString();
        QString name = obj["name"].toString();

        if (m_lobbies.contains(lobbyId)) {
            socket->sendTextMessage(R"({"type":"error","reason":"Lobby already exists"})");
            return;
        }

        Player player{name, socket};
        m_lobbies[lobbyId].append(player);
        m_clientInfo[socket] = qMakePair(lobbyId, name);

        qDebug() << name << "created lobby" << lobbyId;
        broadcastLobbyStatus(lobbyId);
        broadcastLobbyList();
    }
    else if (type == "join") {
        QString lobbyId = obj["lobby"].toString();
        QString name = obj["name"].toString();

        if (!m_lobbies.contains(lobbyId)) {
            socket->sendTextMessage(R"({"type":"error","reason":"Lobby not found"})");
            return;
        }

        if (m_lobbies[lobbyId].size() >= MAX_PLAYERS_PER_LOBBY) {
            socket->sendTextMessage(R"({"type":"disconnect","reason":"Lobby full"})");
            socket->close();
            return;
        }

        Player player{name, socket};
        m_lobbies[lobbyId].append(player);
        m_clientInfo[socket] = qMakePair(lobbyId, name);

        qDebug() << name << "joined lobby" << lobbyId;
        broadcastLobbyStatus(lobbyId);
    }
    else if (type == "leave") {
        removePlayer(socket);
    }
    else if (type == "list") {
        broadcastLobbyList();
    }
}

void LobbyServer::socketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    removePlayer(socket);
    socket->deleteLater();
}

void LobbyServer::removePlayer(QWebSocket *socket)
{
    if (!m_clientInfo.contains(socket))
        return;

    QString lobbyId = m_clientInfo[socket].first;
    QString playerName = m_clientInfo[socket].second;

    QList<Player> &players = m_lobbies[lobbyId];
    auto it = std::remove_if(players.begin(), players.end(), [socket](const Player &p) {
        return p.socket == socket;
    });

    if (it != players.end()) {
        players.erase(it, players.end());
        qDebug() << playerName << "left lobby" << lobbyId;
    }

    m_clientInfo.remove(socket);

    if (players.isEmpty()) {
        m_lobbies.remove(lobbyId);
        qDebug() << "Lobby" << lobbyId << "deleted (empty)";
        broadcastLobbyList();
    } else {
        broadcastLobbyStatus(lobbyId);
    }
}

void LobbyServer::broadcastLobbyStatus(const QString &lobbyId)
{
    if (!m_lobbies.contains(lobbyId)) return;

    QJsonArray playerArray;
    for (const Player &p : m_lobbies[lobbyId])
        playerArray.append(p.name);

    QJsonObject obj;
    obj["type"] = "lobby_update";
    obj["lobby"] = lobbyId;
    obj["players"] = playerArray;

    QJsonDocument doc(obj);
    QString msg = doc.toJson(QJsonDocument::Compact);

    for (const Player &p : m_lobbies[lobbyId])
        p.socket->sendTextMessage(msg);
}

void LobbyServer::broadcastLobbyList()
{
    QJsonArray lobbyArray;
    for (const QString &id : m_lobbies.keys())
        lobbyArray.append(id);

    QJsonObject obj;
    obj["type"] = "lobby_list";
    obj["lobbies"] = lobbyArray;

    QJsonDocument doc(obj);
    QString msg = doc.toJson(QJsonDocument::Compact);

    for (QWebSocket *socket : m_clientInfo.keys()) {
        socket->sendTextMessage(msg);
    }
}
