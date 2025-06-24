// Exemple d'utilisation dans main.cpp
#include <QApplication>
#include "map.h" // Assurez-vous que map.h est inclus

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PacManMap gameMap;
    gameMap.setWindowTitle("PacMan Map");
    gameMap.show();

    return app.exec();
}
