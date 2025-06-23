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
}

void LobbyClient::onDisconnected()
{
    emit connectionFailed("Disconnected from server.");
    qDebug() << "Disconnected.";
}

void LobbyClient::onTextMessageReceived(QString message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    if (obj["type"] == "lobby_update") {
        QStringList players;
        QJsonArray arr = obj["players"].toArray();
        for (auto val : arr)
            players << val.toString();
        emit lobbyUpdated(players);
    }
    else if (message == "Lobby full") {
        emit connectionFailed("Lobby is full");
    }
}

void LobbyClient::sendJoinMessage(const QString &name)
{
    QJsonObject obj;
    obj["type"] = "join";
    obj["name"] = name;
    QJsonDocument doc(obj);
    m_webSocket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
}
