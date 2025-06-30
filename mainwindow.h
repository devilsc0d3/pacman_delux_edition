// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class GameServer;
class GameClient;
class GameScene;
class GameView;
class RulesDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onHostButtonClicked();
    void onJoinButtonClicked();
    void onRulesButtonClicked();

private:
    Ui::MainWindow *ui;

    // Menu widgets
    QWidget* m_menuWidget;
    QPushButton* m_hostButton;
    QPushButton* m_joinButton;
    QPushButton* m_rulesButton;
    QVBoxLayout* m_layout;
    RulesDialog* m_rulesDialog;

    // Game components
    GameScene* m_gameScene;
    GameView* m_gameView;

    // Network components
    GameServer* m_server;
    GameClient* m_client;

    void setupMenu();
    void switchToGameView();
};
#endif // MAINWINDOW_H
