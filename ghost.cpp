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

    m_gridPosition.setX(static_cast<int>(round(x() / TILE_SIZE)));
    m_gridPosition.setY(static_cast<int>(round(y() / TILE_SIZE)));

    // 1. Permettre le demi-tour instantané
    if (m_nextDirection == -m_direction && m_nextDirection != QPoint(0, 0)) {
        m_direction = m_nextDirection;
        m_nextDirection = QPoint(0, 0);
    }

    // 2. Si la direction demandée est possible (pas de mur), l'appliquer dès que possible
    QPointF nextPos = QPointF(x() + m_nextDirection.x() * m_speed, y() + m_nextDirection.y() * m_speed);
    int nextGridX = static_cast<int>(round(nextPos.x() / TILE_SIZE));
    int nextGridY = static_cast<int>(round(nextPos.y() / TILE_SIZE));
    QPoint nextGridPos(nextGridX, nextGridY);
    if (m_nextDirection != QPoint(0, 0) && m_gameMap.cellType(nextGridPos) != Wall) {
        m_direction = m_nextDirection;
        m_nextDirection = QPoint(0, 0);
    }

    // 3. Si la direction actuelle mène à un mur, on stoppe
    QPointF afterMove = QPointF(x() + m_direction.x() * m_speed, y() + m_direction.y() * m_speed);
    int afterGridX = static_cast<int>(round(afterMove.x() / TILE_SIZE));
    int afterGridY = static_cast<int>(round(afterMove.y() / TILE_SIZE));
    QPoint afterGridPos(afterGridX, afterGridY);
    if (m_direction != QPoint(0, 0) && m_gameMap.cellType(afterGridPos) == Wall) {
        setPos(m_gridPosition.x() * TILE_SIZE, m_gridPosition.y() * TILE_SIZE);
        m_direction = QPoint(0, 0);
        return;
    }

    // 4. Appliquer le mouvement
    if (m_direction != QPoint(0, 0)) {
        setPos(x() + m_direction.x() * m_speed, y() + m_direction.y() * m_speed);
    }
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
        if (id == 0) setBrush(QBrush(Qt::yellow)); // Pac-Man 1
        if (id == 1) setBrush(QBrush(Qt::yellow)); // Pac-Man 2
        if (id == 2) setBrush(QBrush(Qt::cyan)); // Fantôme 1
        if (id == 3) setBrush(QBrush(Qt::cyan)); // Fantôme 2
    }
}

int Ghost::getPlayerId() const { return m_playerId; }
bool Ghost::isFrightened() const { return m_state == Frightened; }
