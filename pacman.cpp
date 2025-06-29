// pacman.cpp
#include "pacman.h"
#include <QBrush>
#include <QPen>
#include <QDebug>
#include <cmath>
#include <QGraphicsScene>
#include "ghost.h"

PacMan::PacMan(const GameMap& map)
    : QGraphicsEllipseItem(0, 0, TILE_SIZE, TILE_SIZE),
    m_gameMap(map),
    m_direction(0, 0),
    m_nextDirection(0, 0),
    m_speed(2.0f),
    m_playerId(-1),
    m_lives(3),
    m_isDying(false)
{
    setBrush(QBrush(Qt::magenta));
    setPen(Qt::NoPen);
}

void PacMan::setDirection(const QPoint& direction)
{
    if (m_isDying) return;
    m_nextDirection = direction;
}

void PacMan::advance(int phase)
{
    if (m_isDying || phase == 0) return;
    handleCollisions();
}

void PacMan::handleCollisions()
{
    if (m_isDying) return;

    m_gridPosition.setX(static_cast<int>(round(x() / TILE_SIZE)));
    m_gridPosition.setY(static_cast<int>(round(y() / TILE_SIZE)));

    bool onGrid = (fmod(x(), TILE_SIZE) == 0) && (fmod(y(), TILE_SIZE) == 0);
    if (onGrid && m_nextDirection != QPoint(0, 0)) {
        QPoint nextGridPos = m_gridPosition + m_nextDirection;
        if (m_gameMap.cellType(nextGridPos) != Wall) {
            m_direction = m_nextDirection;
            m_nextDirection = QPoint(0, 0);
        }
    }
    if (onGrid && m_direction != QPoint(0, 0)) {
        QPoint nextGridPos = m_gridPosition + m_direction;
        if (m_gameMap.cellType(nextGridPos) == Wall) {
            m_direction = QPoint(0, 0);
        }
    }
    setPos(x() + m_direction.x() * m_speed, y() + m_direction.y() * m_speed);

    const QList<QGraphicsItem*> collisions = collidingItems();
    for (QGraphicsItem* item : collisions) {
        if (item->type() == GhostType) {
            Ghost* ghost = static_cast<Ghost*>(item);
            if (ghost->isFrightened()) {
                emit ateGhost(ghost);
            } else {
                die();
            }
        } else {
            int itemType = item->data(0).toInt();
            if (itemType == (QGraphicsItem::UserType + 3) ||
                itemType == (QGraphicsItem::UserType + 4))
            {
                emit pelletEaten(item);
            }
        }
    }
}

void PacMan::die()
{
    if (m_isDying) return;
    m_isDying = true;
    m_lives--;
    m_direction = QPoint(0, 0);
    m_nextDirection = QPoint(0, 0);
    emit pacmanDied();
}

void PacMan::resetState()
{
    m_isDying = false;
    m_direction = QPoint(0, 0);
    m_nextDirection = QPoint(0, 0);
}

int PacMan::lives() const { return m_lives; }
void PacMan::setLives(int newLives) { m_lives = newLives; }
void PacMan::setPlayerId(int id) { m_playerId = id; }
int PacMan::getPlayerId() const { return m_playerId; }
