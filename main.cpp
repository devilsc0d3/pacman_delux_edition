#include "mainwindow.h"

#include <QApplication>
#include "lobbyserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     LobbyServer server(1234);
    MainWindow w;
    w.show();
    return a.exec();





}
