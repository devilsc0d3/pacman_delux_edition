#include "ghost.h"
#include <QBrush>
#include <QPen>
#include <QRandomGenerator>
#include <QtMath>
#include <QDebug>

Ghost::Ghost(const GameMap& map)
    : QGraphicsEllipseItem(0, 0, TILE_SIZE, TILE_SIZE),
    m_gameMap(map),
    m_direction(0, 0),
    m_speed(1.8f),
    m_state(Chasing),
    m_playerId(-1),
    m_isPlayerControlled(false)
{
    setBrush(QBrush(Qt::red));
    setPen(Qt::NoPen);
}

void Ghost::setDirection(const QPoint &direction)
{
    if (m_isPlayerControlled) {
        m_direction = direction;
    }
}

void Ghost::advance(int phase)
{
    if (phase == 0) return;

    // 1. Déterminer la direction (IA ou joueur)
    updateDirection();

    // 2. Vérifier les collisions avec les murs AVANT de bouger
    bool onGrid = (fmod(x(), TILE_SIZE) == 0) && (fmod(y(), TILE_SIZE) == 0);
    if (onGrid && m_direction != QPoint(0,0)) {
        QPoint currentGridPos(round(x()/TILE_SIZE), round(y()/TILE_SIZE));
        QPoint nextGridPos = currentGridPos + m_direction;
        if (m_gameMap.cellType(nextGridPos) == Wall) {
            m_direction = QPoint(0, 0); // Arrêt net
        }
    }

    // 3. Appliquer le mouvement (ou l'arrêt)
    setPos(x() + m_direction.x() * m_speed, y() + m_direction.y() * m_speed);
}

void Ghost::updateDirection()
{
    if(m_isPlayerControlled) return;

    bool onGrid = (fmod(x(), TILE_SIZE) == 0) && (fmod(y(), TILE_SIZE) == 0);
    if (onGrid) {
        m_gridPosition.setX(round(x() / TILE_SIZE));
        m_gridPosition.setY(round(y() / TILE_SIZE));
        QPoint nextGridPos = m_gridPosition + m_direction;
        if (m_direction == QPoint(0,0) || m_gameMap.cellType(nextGridPos) == Wall) {
            QVector<QPoint> possibleDirections;
            if (m_gameMap.cellType(m_gridPosition + QPoint(0, -1)) != Wall) possibleDirections.append(QPoint(0, -1));
            if (m_gameMap.cellType(m_gridPosition + QPoint(0, 1))  != Wall) possibleDirections.append(QPoint(0, 1));
            if (m_gameMap.cellType(m_gridPosition + QPoint(-1, 0)) != Wall) possibleDirections.append(QPoint(-1, 0));
            if (m_gameMap.cellType(m_gridPosition + QPoint(1, 0))  != Wall) possibleDirections.append(QPoint(1, 0));
            if (possibleDirections.size() > 1 && m_state != Frightened) {
                possibleDirections.removeOne(QPoint(-m_direction.x(), -m_direction.y()));
            }
            if (!possibleDirections.isEmpty()) {
                m_direction = possibleDirections[QRandomGenerator::global()->bounded(possibleDirections.size())];
            } else {
                m_direction = -m_direction;
            }
        }
    }
}

void Ghost::setFrightened(bool isFrightened)
{
    if (m_state == Eaten) return;
    if (isFrightened) {
        m_state = Frightened; m_speed = 1.2f; setBrush(QBrush(Qt::darkBlue));
    } else {
        m_state = Chasing; m_speed = 1.8f; setBrush(QBrush(Qt::red));
    }
}

void Ghost::setEaten()
{
    m_state = Chasing;
    setBrush(QBrush(Qt::red));
    m_speed = 1.8f;
}

void Ghost::setPlayerId(int id)
{
    m_playerId = id;
    // Les joueurs 2 et 3 contrôlent les fantômes
    if (id == 2 || id == 3) {
        m_isPlayerControlled = true;
    }
    // Change la couleur pour différencier les fantômes
    if (id == 0) setBrush(QBrush(Qt::red)); // Fantôme IA 1
    if (id == 1) setBrush(QBrush(Qt::cyan)); // Fantôme IA 2
    if (id == 2) setBrush(QBrush(Qt::green)); // Joueur 3
    if (id == 3) setBrush(QBrush(QColor(255, 165, 0))); // Joueur 4 (Orange)
}

int Ghost::getPlayerId() const { return m_playerId; }
bool Ghost::isFrightened() const { return m_state == Frightened; }
