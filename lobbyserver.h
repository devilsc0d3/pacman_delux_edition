#ifndef LOBBYSERVER_H
#define LOBBYSERVER_H

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QMap>
#include <QSet>

struct Player {
    QString name;
    QWebSocket* socket;
};

class LobbyServer : public QObject
{
    Q_OBJECT
public:
    explicit LobbyServer(quint16 port, QObject *parent = nullptr);
    ~LobbyServer();

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();

private:
    QWebSocketServer *m_server;

    QMap<QString, QList<Player>> m_lobbies;
    QMap<QWebSocket*, QPair<QString, QString>> m_clientInfo;

    const int MAX_PLAYERS_PER_LOBBY = 4;

    void broadcastLobbyList();
    void broadcastLobbyStatus(const QString &lobbyId);
    void removePlayer(QWebSocket* socket);
};

#endif
