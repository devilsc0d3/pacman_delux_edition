#include <QApplication>
#include <QMainWindow> // Ou juste QWidget
#include "map.h"
#include "MapView.h"
// Incluez d'autres fichiers si nécessaire, comme PacGumFactory

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Créer l'objet Map (le modèle de données)
    Map gameMap;

    // 2. Créer l'objet MapView (la vue graphique) en lui passant le modèle
    MapView mapView(gameMap);

    // Facultatif : Définir une taille de cellule différente si vous voulez
    // mapView.setCellSize(25);

    // Facultatif : Utiliser la Map avec la PacGumFactory (comme prévu)
    // PacGumFactory factory;
    // QVector<GumEntity> gums = factory.populateMap(gameMap, BonusGenerationStrategy::BonusOnCorner);
    // Vous auriez besoin de stocker et gérer ces gums, et potentiellement les dessiner aussi.

    // 3. Créer une fenêtre principale pour contenir la vue
    QMainWindow window;
    window.setWindowTitle("Pac-Man Map");
    window.setCentralWidget(&mapView); // Placez la MapView dans la fenêtre centrale

    // 4. Afficher la fenêtre
    window.show();

    // 5. Lancer la boucle d'événements de l'application Qt
    return a.exec();
}
