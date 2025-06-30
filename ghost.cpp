// ghost.cpp
#include "ghost.h"
#include <QBrush>
#include <QPen>
#include <QtMath>
#include <QDebug>

Ghost::Ghost(const GameMap& map)
    : QGraphicsEllipseItem(0, 0, TILE_SIZE, TILE_SIZE),
    m_gameMap(map),
    m_direction(0, 0),
    m_nextDirection(0, 0), // Initialiser la direction demandée
    m_speed(1.8f),
    m_state(Chasing),
    m_playerId(-1)
{
    setBrush(QBrush(Qt::red));
    setPen(Qt::NoPen);
}

void Ghost::setDirection(const QPoint &direction)
{
    // On stocke simplement la direction demandée par le joueur.
    m_nextDirection = direction;
}

void Ghost::advance(int phase)
{
    if (phase == 0) return;

    bool onGrid = (fmod(x(), TILE_SIZE) == 0) && (fmod(y(), TILE_SIZE) == 0);
    m_gridPosition.setX(round(x()/TILE_SIZE));
    m_gridPosition.setY(round(y()/TILE_SIZE));

    if (onGrid) {
        // 1. Appliquer la prochaine direction si elle est valide
        if (m_nextDirection != QPoint(0,0)) {
            QPoint nextGridPos = m_gridPosition + m_nextDirection;
            if (m_gameMap.cellType(nextGridPos) != Wall) {
                m_direction = m_nextDirection;
                m_nextDirection = QPoint(0,0); // Réinitialiser la demande
            }
        }

        // 2. Vérifier la collision pour la direction actuelle
        if (m_direction != QPoint(0,0)) {
            QPoint nextGridPos = m_gridPosition + m_direction;
            if (m_gameMap.cellType(nextGridPos) == Wall) {
                m_direction = QPoint(0, 0); // Arrêt net
            }
        }
    }

    // 3. Appliquer le mouvement
    setPos(x() + m_direction.x() * m_speed, y() + m_direction.y() * m_speed);
}

void Ghost::setFrightened(bool isFrightened)
{
    if (m_state == Eaten) return;
    if (isFrightened) {
        if(m_state != Frightened) m_direction = -m_direction; // Demi-tour
        m_state = Frightened; m_speed = 1.2f; setBrush(QBrush(Qt::darkBlue));
    } else {
        m_state = Chasing; m_speed = 1.8f;
        setPlayerId(m_playerId); // Rétablir la couleur
    }
}

void Ghost::setEaten()
{
    m_state = Chasing;
    m_speed = 1.8f;
    setPlayerId(m_playerId);
}

void Ghost::setPlayerId(int id)
{
    m_playerId = id;
    if (m_state != Frightened) {
        // Couleurs par défaut pour les 4 joueurs
        if (id == 0) setBrush(QBrush(Qt::magenta)); // Pac-Man 1
        if (id == 1) setBrush(QBrush(QColor(255, 182, 193))); // Pac-Man 2
        if (id == 2) setBrush(QBrush(Qt::red)); // Fantôme 1
        if (id == 3) setBrush(QBrush(Qt::cyan)); // Fantôme 2
    }
}

int Ghost::getPlayerId() const { return m_playerId; }
bool Ghost::isFrightened() const { return m_state == Frightened; }
