#ifndef LOBBYCLIENT_H
#define LOBBYCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>

enum class PendingActionType { None, Create, Join };

class LobbyClient : public QObject
{
    Q_OBJECT
public:
    explicit LobbyClient(const QUrl &url, QObject *parent = nullptr);

    void sendCreate(const QString &lobbyId, const QString &name);
    void sendJoin(const QString &lobbyId, const QString &name);
    void sendLeave();
    void requestLobbyList();

    QString pendingName;
    QString pendingLobby;
    PendingActionType pendingAction = PendingActionType::None;

signals:
    void lobbyUpdated(QString lobbyId, QStringList players);
    void lobbyListReceived(QStringList lobbies);
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

#endif // LOBBYCLIENT_H
