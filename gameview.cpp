// gameview.cpp
#include "gameview.h"
#include "gamemap.h"
#include <QDebug>
#include <QKeyEvent> // S'assurer que QKeyEvent est inclus pour keyPressEvent

GameView::GameView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView{scene, parent}
{
    setRenderHint(QPainter::Antialiasing);

    // On dit à la vue de prendre toute la place disponible.
    // C'est plus flexible qu'une taille fixe.
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::StrongFocus);
}

void GameView::keyPressEvent(QKeyEvent *event)
{
    // Ne pas traiter de nouvelles pressions si la touche est déjà enfoncée (évite le spam)
    if (event->isAutoRepeat()) {
        return;
    }

    QPoint direction;
    switch (event->key()) {
    case Qt::Key_Up:
        direction = QPoint(0, -1);
        break;
    case Qt::Key_Down:
        direction = QPoint(0, 1);
        break;
    case Qt::Key_Left:
        direction = QPoint(-1, 0);
        break;
    case Qt::Key_Right:
        direction = QPoint(1, 0);
        break;
    default:
        // Laisser le parent gérer les autres touches (ex: Alt+F4)
        QGraphicsView::keyPressEvent(event);
        return;
    }

    // Émettre un signal pour dire que le joueur 0 (pour l'instant) a donné une direction
    // Le playerID sera géré plus tard par le serveur.
    emit directionInput(0, direction);
}
