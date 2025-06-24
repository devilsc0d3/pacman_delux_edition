#include "lobbyclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

LobbyClient::LobbyClient(const QUrl &url, QObject *parent)
    : QObject(parent), m_url(url)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &LobbyClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &LobbyClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &LobbyClient::onTextMessageReceived);

    m_webSocket.open(m_url);
}

void LobbyClient::onConnected()
{
    emit connectionSuccess();
    qDebug() << "Connected to server.";

    // Rejouer l'action prévue si nécessaire
    if (pendingAction == PendingActionType::Create) {
        sendCreate(pendingLobby, pendingName);
    } else if (pendingAction == PendingActionType::Join) {
        sendJoin(pendingLobby, pendingName);
    }

    requestLobbyList();
}

void LobbyClient::onDisconnected()
{
    emit connectionFailed("Disconnected from server.");
    qDebug() << "Disconnected.";
}

void LobbyClient::sendCreate(const QString &lobbyId, const QString &name)
{
    pendingAction = PendingActionType::Create;
    pendingName = name;
    pendingLobby = lobbyId;

    if (m_webSocket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject obj;
        obj["type"] = "create";
        obj["lobby"] = lobbyId;
        obj["name"] = name;
        QJsonDocument doc(obj);
        m_webSocket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void LobbyClient::sendJoin(const QString &lobbyId, const QString &name)
{
    pendingAction = PendingActionType::Join;
    pendingName = name;
    pendingLobby = lobbyId;

    if (m_webSocket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject obj;
        obj["type"] = "join";
        obj["lobby"] = lobbyId;
        obj["name"] = name;
        QJsonDocument doc(obj);
        m_webSocket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    }
}

void LobbyClient::sendLeave()
{
    QJsonObject obj;
    obj["type"] = "leave";
    QJsonDocument doc(obj);
    m_webSocket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void LobbyClient::requestLobbyList()
{
    QJsonObject obj;
    obj["type"] = "list";
    QJsonDocument doc(obj);
    m_webSocket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void LobbyClient::onTextMessageReceived(QString message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    QString type = obj["type"].toString();

    if (type == "lobby_update") {
        QString lobbyId = obj["lobby"].toString();
        QStringList players;
        QJsonArray arr = obj["players"].toArray();
        for (const auto &val : arr)
            players << val.toString();
        emit lobbyUpdated(lobbyId, players);
    }
    else if (type == "lobby_list") {
        QStringList lobbies;
        QJsonArray arr = obj["lobbies"].toArray();
        for (const auto &val : arr)
            lobbies << val.toString();
        emit lobbyListReceived(lobbies);
    }
    else if (type == "disconnect") {
        emit connectionFailed(obj["reason"].toString());
        m_webSocket.close();
    }
    else if (type == "error") {
        emit connectionFailed(obj["reason"].toString());
    }
}
