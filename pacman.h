// pacman.h
#ifndef PACMAN_H
#define PACMAN_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include "gamemap.h"

class QGraphicsItem;
class Ghost;

const int PacManType = QGraphicsItem::UserType + 1;

class PacMan : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    explicit PacMan(const GameMap& map);

    int type() const override { return PacManType; }
    void advance(int phase) override;

    void die();
    void resetState();
    int lives() const;
    void setLives(int newLives);

    void setPlayerId(int id);
    int getPlayerId() const;

signals:
    void pelletEaten(QGraphicsItem* pelletItem);
    void ateGhost(Ghost* ghost);
    void pacmanDied();

public slots:
    void setDirection(const QPoint& direction);

private:
    const GameMap& m_gameMap;
    QPoint m_gridPosition;
    QPoint m_direction;
    QPoint m_nextDirection;
    float m_speed;

    int m_playerId;
    int m_lives;
    bool m_isDying;

    void handleCollisions();
};

#endif // PACMAN_H
