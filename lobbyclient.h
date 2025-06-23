#ifndef LOBBYCLIENT_H
#define LOBBYCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

class LobbyClient : public QObject
{
    Q_OBJECT
public:
    explicit LobbyClient(const QUrl &url, QObject *parent = nullptr);
    void sendJoinMessage(const QString &name);

signals:
    void lobbyUpdated(QStringList players);
    void connectionSuccess();
    void connectionFailed(QString reason);

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onDisconnected();

private:
    QWebSocket m_webSocket;
    QUrl m_url;
};

#endif 
