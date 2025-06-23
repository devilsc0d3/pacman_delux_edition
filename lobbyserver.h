#ifndef LOBBYSERVER_H
#define LOBBYSERVER_H

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

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
    QList<Player> m_players;
    const int MAX_PLAYERS = 4;

    void broadcastLobbyStatus();
};

#endif // LOBBYSERVER_H
