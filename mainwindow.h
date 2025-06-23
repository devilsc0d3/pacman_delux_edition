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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_buttonConnect_clicked();
    void updateLobby(QList<QString> players);
    void onConnectionSuccess();
    void onConnectionFailed(QString reason);

private:
    Ui::MainWindow *ui;
    LobbyClient *client = nullptr;
};
#endif // MAINWINDOW_H
