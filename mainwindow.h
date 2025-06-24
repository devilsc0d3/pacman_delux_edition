#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lobbyclient.h" // Nécessaire pour le membre 'client'

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
    // Slots de la branche feat/Server
    void onCreateLobby();
    void onJoinLobby();
    void onLeaveLobby();
    void onRefreshLobbies();
    void onConnectionSuccess();
    void onConnectionFailed(QString reason);
    void onLobbyUpdated(QString lobbyId, QStringList players);
    void onLobbyListReceived(QStringList lobbies);

    // Slot de la branche feat/map
    void on_pushButton_clicked();

private:
    void startClient();

    Ui::MainWindow *ui;
    LobbyClient *client = nullptr;
};

#endif // MAINWINDOW_H