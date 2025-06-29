// gameserver.h
#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QTimer>
#include <QPoint>

#include "gamescene.h"
#include "networkprotocol.h"

class QTcpSocket;

class GameServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);
    void startServer(quint16 port);

private slots:
    void updateAndBroadcastState();
    void onLevelCleared();
    void onBroadcastPelletEaten(const QPoint& gridPos);
    void onPacmanKilled(); // La déclaration qui manquait

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    struct ClientInfo {
        QTcpSocket* socket;
        int playerId;
    };
    QList<ClientInfo> m_clients;

    GameScene* m_gameScene;
    QTimer* m_broadcastTimer;
    int m_nextPlayerId;

    void sendStateToClient(QTcpSocket* client);
    void handleClientData(QTcpSocket* client);
};

#endif // GAMESERVER_H
