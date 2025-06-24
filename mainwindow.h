#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lobbyclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onCreateLobby();
    void onJoinLobby();
    void onLeaveLobby();

    void onConnectionSuccess();
    void onConnectionFailed(QString reason);
    void onLobbyUpdated(QString lobbyId, QStringList players);
    void onLobbyListReceived(QStringList lobbies);
    void onRefreshLobbies();

private:
    void startClient();

    Ui::MainWindow *ui;
    LobbyClient *client = nullptr;
};



#endif
