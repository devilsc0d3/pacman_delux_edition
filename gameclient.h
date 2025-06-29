// gameclient.h
#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "networkprotocol.h"

class GameClient : public QObject
{
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);
    void connectToServer(const QString& hostAddress, quint16 port);
    void sendPlayerInput(const QPoint& direction);

signals:
    void connected();
    void disconnected();
    void gameStateReceived(const GameState& state);
    void levelClearedOnClient();
    void pelletRemoved(const QPoint& gridPos);
    void resetLevelOnClient();
private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();

private:
    QTcpSocket* m_socket;
    int m_myPlayerId;
};

#endif // GAMECLIENT_H
