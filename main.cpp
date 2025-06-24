#include <QApplication>
#include <QMainWindow>

// Fichiers des différentes branches
#include "mainwindow.h"
#include "lobbyserver.h"
#include "map.h"
#include "MapView.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialisation du serveur depuis la branche feat/Server
    LobbyServer server(1234);

    // Création et affichage de la fenêtre principale
    MainWindow w;
    w.show();

    // Création de la carte et de sa vue
    Map gameMap;
    // CORRECTION APPLIQUÉE ICI : On passe l'objet directement, pas son adresse.
    MapView mapView(gameMap);

    // Création et affichage de la fenêtre pour la carte
    QMainWindow mapDisplayWindow;
    mapDisplayWindow.setWindowTitle("Affichage de la carte");
    mapDisplayWindow.setCentralWidget(&mapView);
    mapDisplayWindow.resize(800, 600);
    mapDisplayWindow.show();

    return a.exec();
}
