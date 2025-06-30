// ghost.h
#ifndef GHOST_H
#define GHOST_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include "gamemap.h"

const int GhostType = QGraphicsItem::UserType + 2;

class Ghost : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    enum GhostState { Chasing, Frightened, Eaten };
    explicit Ghost(const GameMap& map);

    int type() const override { return GhostType; }
    void advance(int phase) override;

    void setFrightened(bool isFrightened);
    bool isFrightened() const;
    void setEaten();

    void setPlayerId(int id);
    int getPlayerId() const;
    void setDirection(const QPoint& direction);

private:
    const GameMap& m_gameMap;
    QPoint m_gridPosition;
    QPoint m_direction;
    QPoint m_nextDirection; // Pour le contrôle joueur (comme Pac-Man)
    float m_speed;
    GhostState m_state;
    int m_playerId;
};

#endif // GHOST_H
