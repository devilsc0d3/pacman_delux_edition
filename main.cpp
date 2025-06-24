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
    // Il s'exécutera en arrière-plan.
    LobbyServer server(1234);

    // Création et affichage de la fenêtre principale (comportement de base)
    MainWindow w;
    w.show();

    // Création et affichage de la fenêtre de test pour la carte depuis la branche feat/map
    // NOTE : Ceci ouvre une DEUXIÈME fenêtre.
    Map gameMap;
    MapView mapView(&gameMap); // Le constructeur attend peut-être un pointeur
    QMainWindow mapDisplayWindow;
    mapDisplayWindow.setWindowTitle("Affichage de la carte");
    mapDisplayWindow.setCentralWidget(&mapView);
    mapDisplayWindow.resize(800, 600); // Ajout d'une taille par défaut pour la visibilité
    mapDisplayWindow.show();

    return a.exec();
}