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
    m_lives(1),
    m_isActive(true)
{
    setBrush(QBrush(Qt::magenta));
    setPen(Qt::NoPen);
}

void PacMan::setDirection(const QPoint& direction)
{
    if (!m_isActive) return;
    m_nextDirection = direction;
}

void PacMan::advance(int phase)
{
    if (!m_isActive || phase == 0) return;
    handleCollisions();
}

void PacMan::handleCollisions()
{
    if (!m_isActive) return;

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
    bool ateFrightenedGhost = false;
    for (QGraphicsItem* item : collisions) {
        if (!item->isVisible()) continue;
        if (item->type() == GhostType) {
            Ghost* ghost = static_cast<Ghost*>(item);
            if (ghost->isFrightened()) {
                emit ateGhost(ghost);
                ateFrightenedGhost = true;
            }
        }
    }
    if (!ateFrightenedGhost) {
        for (QGraphicsItem* item : collisions) {
            if (!item->isVisible()) continue;
            if (item->type() == GhostType) {
                Ghost* ghost = static_cast<Ghost*>(item);
                if (!ghost->isFrightened()) {
                    die();
                    break;
                }
            }
        }
    }
    for (QGraphicsItem* item : collisions) {
        if (!item->isVisible()) continue;
        int itemType = item->data(0).toInt();
        if (itemType == (QGraphicsItem::UserType + 3) ||
            itemType == (QGraphicsItem::UserType + 4))
        {
            emit pelletEaten(item);
        }
    }
}

void PacMan::die()
{
    if (!m_isActive) return;
    m_isActive = false;
    m_lives = 0;
    setVisible(false);
    emit pacmanDied();
}

void PacMan::resetState()
{
    m_isActive = true;
    m_lives = 1;
    setVisible(true);
    m_direction = QPoint(0, 0);
    m_nextDirection = QPoint(0, 0);
}

bool PacMan::isActive() const { return m_isActive; }
int PacMan::lives() const { return m_lives; }
void PacMan::setLives(int newLives) { m_lives = newLives; }
void PacMan::setPlayerId(int id) { m_playerId = id; }
int PacMan::getPlayerId() const { return m_playerId; }
