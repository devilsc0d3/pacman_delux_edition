#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lobbyclient.h" // On garde cet include car il est essentiel au lobby

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
    // On conserve tous les slots liés à la fonctionnalité de LOBBY
    void onCreateLobby();
    void onJoinLobby();
    void onLeaveLobby();
    void onRefreshLobbies();
    void onConnectionSuccess();
    void onConnectionFailed(QString reason);
    void onLobbyUpdated(QString lobbyId, QStringList players);
    void onLobbyListReceived(QStringList lobbies);
    void on_pushButton_clicked(); // Pour le bouton "Règles" dans le lobby

private:
    // On conserve les membres privés nécessaires au LOBBY
    void startClient();

    Ui::MainWindow *ui;
    LobbyClient *client = nullptr; // Essentiel pour la communication avec le serveur
};

#endif // MAINWINDOW_H