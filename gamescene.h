// gamescene.h
#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QPoint>
#include <QGraphicsTextItem>
#include <QHash>

#include "gamemap.h"
#include "pacman.h"
#include "ghost.h"
#include "networkprotocol.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();

    void startGame();
    QList<CharacterState> getPacmanStates() const;
    QList<CharacterState> getGhostStates() const;
    int getScore() const;
    void reloadLevel();
    QTimer* getGameTimer() const;
    void gameOver(); // Déplacé ici pour être public

signals:
    void levelWasCleared();
    void pelletWasEatenAt(const QPoint& gridPos);
    void allPacmansDied();

public slots:
    void updateFromServer(const GameState& state);
    void setPacmanDirection(int playerID, const QPoint& direction);
    void levelCleared();
    void onPelletRemoved(const QPoint& gridPos);
    void resetLevelClient();

private slots:
    void updateGame();
    void onPelletEaten(QGraphicsItem* pelletItem);
    void onGhostEaten(Ghost* ghost);
    void onFrightenTimerTimeout();
    void onPacmanDied();

private:
    GameMap m_gameMap;
    QTimer *m_gameTimer;
    QTimer *m_frightenTimer;

    QList<PacMan*> m_pacmans;
    QList<Ghost*> m_ghosts;
    QHash<QPoint, QGraphicsItem*> m_pelletItems;

    int m_score;
    int m_pelletCount;
    QGraphicsTextItem* m_scoreText;
    QGraphicsTextItem* m_livesText;

    void loadMap();
    void clearScene();
    void setupCharacters();
    void updateScoreDisplay();
    void updateLivesDisplay();
    void resetLevel();
    // gameOver() a été déplacé vers public
};

#endif // GAMESCENE_H
