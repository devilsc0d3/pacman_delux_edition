#include <QApplication>
#include <QMainWindow> // Ou juste QWidget
#include "map.h"
#include "MapView.h"
#include "mainwindow.h"
// Incluez d'autres fichiers si nécessaire, comme PacGumFactory

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Map gameMap;
    MapView mapView(gameMap);
    QMainWindow window;
    window.setWindowTitle("Pac-Man Map");
    window.setCentralWidget(&mapView);
    MainWindow w;
    window.show();
    w.show();
    return a.exec();
}
