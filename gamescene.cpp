// gamescene.cpp
#include "gamescene.h"
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QDebug>
#include "networkprotocol.h"

const int PelletType = QGraphicsItem::UserType + 3;
const int PowerPelletType = QGraphicsItem::UserType + 4;

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent},
    m_gameTimer(nullptr),
    m_frightenTimer(nullptr),
    m_deathPauseTimer(nullptr),
    m_score(0),
    m_pelletCount(0),
    m_scoreText(nullptr),
    m_livesText(nullptr)
{
    setSceneRect(0, -TILE_SIZE * 2, MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE + TILE_SIZE * 2);
    setBackgroundBrush(Qt::black);
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, &GameScene::updateGame);
    m_frightenTimer = new QTimer(this);
    m_frightenTimer->setSingleShot(true);
    connect(m_frightenTimer, &QTimer::timeout, this, &GameScene::onFrightenTimerTimeout);
    m_deathPauseTimer = new QTimer(this);
    m_deathPauseTimer->setSingleShot(true);
    connect(m_deathPauseTimer, &QTimer::timeout, this, &GameScene::finishDeathSequence);
    loadMap();
    setupCharacters();
}

GameScene::~GameScene() { }
void GameScene::startGame() { m_gameTimer->start(1000 / 60); }
void GameScene::updateGame() { advance(); }
QTimer* GameScene::getGameTimer() const { return m_gameTimer; }

void GameScene::updateFromServer(const GameState &state)
{
    for(const CharacterState& pacmanState : state.pacmanStates) {
        PacMan* targetPacman = nullptr;
        for(PacMan* p : m_pacmans) {
            if (p->getPlayerId() == pacmanState.id) {
                targetPacman = p;
                break;
            }
        }
        if (targetPacman) {
            targetPacman->setPos(pacmanState.position);
        }
    }
    for(const CharacterState& ghostState : state.ghostStates) {
        Ghost* targetGhost = nullptr;
        for(Ghost* g : m_ghosts) {
            if (g->getPlayerId() == ghostState.id) {
                targetGhost = g;
                break;
            }
        }
        if (targetGhost) {
            targetGhost->setPos(ghostState.position);
        }
    }
    m_score = state.score;
    updateScoreDisplay();
}

void GameScene::setPacmanDirection(int playerID, const QPoint& direction)
{
    if (playerID >= 0 && playerID < 2) {
        for(PacMan* pacman : m_pacmans) {
            if (pacman->getPlayerId() == playerID) {
                pacman->setDirection(direction);
                return;
            }
        }
    } else if (playerID >= 2 && playerID < 4) {
        for(Ghost* ghost : m_ghosts) {
            if (ghost->getPlayerId() == playerID) {
                ghost->setDirection(direction);
                return;
            }
        }
    }
}

void GameScene::onPelletEaten(QGraphicsItem* pelletItem)
{
    if (!pelletItem || !items().contains(pelletItem)) return;

    QPoint gridPos(-1,-1);
    for(auto it = m_pelletItems.constBegin(); it != m_pelletItems.constEnd(); ++it) {
        if (it.value() == pelletItem) {
            gridPos = it.key();
            break;
        }
    }

    if (gridPos == QPoint(-1,-1)) {
        qWarning() << "[Serveur] Bille mangée non trouvée dans la table de hachage !";
        // On supprime quand même l'item pour éviter les bugs, mais on ne peut pas notifier
        delete pelletItem;
        return;
    }

    int itemType = pelletItem->data(0).toInt();
    if (itemType == PelletType) m_score += 10;
    else if (itemType == PowerPelletType) {
        m_score += 50;
        for(Ghost* ghost : m_ghosts) ghost->setFrightened(true);
        m_frightenTimer->start(7000);
    }
    updateScoreDisplay();

    m_pelletItems.remove(gridPos);
    delete pelletItem;

    emit pelletWasEatenAt(gridPos);

    m_pelletCount--;
    if (m_pelletCount <= 0) {
        emit levelWasCleared();
    }
}

void GameScene::onPelletRemoved(const QPoint &gridPos)
{
    qDebug() << "[Client] Tentative de suppression de la bille à" << gridPos;
    if (m_pelletItems.contains(gridPos)) {
        QGraphicsItem* pellet = m_pelletItems.take(gridPos);
        if (pellet) {
            delete pellet;
            qDebug() << "[Client] Bille supprimée avec succès.";
        }
    } else {
        qWarning() << "[Client] Impossible de trouver la bille à supprimer à la position" << gridPos;
    }
}

void GameScene::onGhostEaten(Ghost* ghost)
{
    if (!ghost) return;
    m_score += 200;
    updateScoreDisplay();
    ghost->setEaten();
    int ghostIndex = ghost->getPlayerId() - 2;
    if (ghostIndex >= 0 && ghostIndex < 2) {
        QPoint ghostSpawn = m_gameMap.getGhostSpawnPoint(ghostIndex);
        ghost->setPos(ghostSpawn.x() * TILE_SIZE, ghostSpawn.y() * TILE_SIZE);
    }
}

void GameScene::onFrightenTimerTimeout()
{
    for(Ghost* ghost : m_ghosts) {
        ghost->setFrightened(false);
    }
}

void GameScene::startDeathSequence()
{
    PacMan* deadPacman = qobject_cast<PacMan*>(sender());
    if(deadPacman) { }
    m_gameTimer->stop();
    m_deathPauseTimer->start(2000);
    emit pacmanWasKilled();
}

void GameScene::finishDeathSequence()
{
    if (!m_pacmans.isEmpty() && m_pacmans.first()->lives() > 0) {
        resetLevel();
        m_gameTimer->start(1000/60);
    } else {
        gameOver();
    }
    updateLivesDisplay();
}

void GameScene::updateScoreDisplay() { if(m_scoreText) m_scoreText->setPlainText(QString("Score: %1").arg(m_score)); }
void GameScene::updateLivesDisplay() { if(!m_pacmans.isEmpty() && m_livesText) m_livesText->setPlainText(QString("Vies J1: %1").arg(m_pacmans.first()->lives())); }

void GameScene::resetLevelClient()
{
    // Le client recharge toute la carte et réinitialise les personnages
    loadMap();
    setupCharacters();
}
void GameScene::resetLevel()
{
    for(PacMan* pacman : m_pacmans) {
        QPoint spawn = m_gameMap.getPacManSpawnPoint(pacman->getPlayerId());
        pacman->setPos(spawn.x() * TILE_SIZE, spawn.y() * TILE_SIZE);
        pacman->resetState();
    }
    for(Ghost* ghost : m_ghosts) {
        int ghostIndex = ghost->getPlayerId() - 2;
        if (ghostIndex >= 0 && ghostIndex < 2) {
            QPoint spawn = m_gameMap.getGhostSpawnPoint(ghostIndex);
            ghost->setPos(spawn.x() * TILE_SIZE, ghost->y() * TILE_SIZE);
            ghost->setFrightened(false);
        }
    }
}

void GameScene::gameOver()
{
    m_gameTimer->stop();
    QGraphicsTextItem* gameOverText = new QGraphicsTextItem("GAME OVER");
    gameOverText->setDefaultTextColor(Qt::red);
    gameOverText->setFont(QFont("Arial", 40, QFont::Bold));
    gameOverText->setPos( (sceneRect().width() - gameOverText->boundingRect().width()) / 2,
                         (sceneRect().height() - gameOverText->boundingRect().height()) / 2 - TILE_SIZE * 2);
    addItem(gameOverText);
}

void GameScene::levelCleared()
{
    QGraphicsTextItem* levelText = new QGraphicsTextItem("NIVEAU TERMINE !");
    levelText->setDefaultTextColor(Qt::green);
    levelText->setFont(QFont("Arial", 40, QFont::Bold));
    levelText->setPos( (sceneRect().width() - levelText->boundingRect().width()) / 2, (sceneRect().height() - levelText->boundingRect().height()) / 2 - TILE_SIZE * 2);
    addItem(levelText);
    QTimer::singleShot(2800, this, [levelText](){
        if(levelText) delete levelText;
    });
}

void GameScene::reloadLevel()
{
    loadMap();
    resetLevel();
}

void GameScene::setupCharacters()
{
    for (int i = 0; i < 2; ++i) {
        PacMan* pacman = new PacMan(m_gameMap);
        pacman->setPlayerId(i);
        QPoint spawn = m_gameMap.getPacManSpawnPoint(i);
        pacman->setPos(spawn.x() * TILE_SIZE, spawn.y() * TILE_SIZE);
        if (i == 1) pacman->setBrush(QBrush(QColor(255, 182, 193)));
        m_pacmans.append(pacman);
        addItem(pacman);
        connect(pacman, &PacMan::pelletEaten, this, &GameScene::onPelletEaten, Qt::QueuedConnection);
        connect(pacman, &PacMan::ateGhost, this, &GameScene::onGhostEaten, Qt::QueuedConnection);
        connect(pacman, &PacMan::pacmanDied, this, &GameScene::startDeathSequence, Qt::QueuedConnection);
    }
    if (!m_pacmans.isEmpty()) updateLivesDisplay();

    for (int i = 0; i < 2; ++i) {
        Ghost* ghost = new Ghost(m_gameMap);
        ghost->setPlayerId(i + 2);
        QPoint spawn = m_gameMap.getGhostSpawnPoint(i);
        ghost->setPos(spawn.x() * TILE_SIZE, spawn.y() * TILE_SIZE);
        m_ghosts.append(ghost);
        addItem(ghost);
    }
}

void GameScene::loadMap()
{
    clearScene();
    m_pelletCount = 0;
    QBrush wallBrush(Qt::blue);
    QPen noPen(Qt::NoPen);
    QBrush pelletBrush(Qt::white);
    QPen pelletPen(Qt::NoPen);
    QBrush powerPelletBrush(Qt::yellow);
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            CellType type = m_gameMap.cellType(x, y);
            qreal cellX = x * TILE_SIZE;
            qreal cellY = y * TILE_SIZE;
            switch (type) {
            case Wall: {
                QGraphicsRectItem *wall = new QGraphicsRectItem(cellX, cellY, TILE_SIZE, TILE_SIZE);
                wall->setBrush(wallBrush); wall->setPen(noPen); addItem(wall); break;
            }
            case Pellet: {
                qreal pelletSize = TILE_SIZE / 4;
                QGraphicsEllipseItem *pellet = new QGraphicsEllipseItem(cellX + (TILE_SIZE - pelletSize) / 2, cellY + (TILE_SIZE - pelletSize) / 2, pelletSize, pelletSize);
                pellet->setBrush(pelletBrush); pellet->setPen(pelletPen);
                pellet->setData(0, PelletType);
                addItem(pellet);
                m_pelletItems[QPoint(x,y)] = pellet;
                m_pelletCount++;
                break;
            }
            case PowerPellet: {
                qreal powerPelletSize = TILE_SIZE / 2;
                QGraphicsEllipseItem *powerPellet = new QGraphicsEllipseItem(cellX + (TILE_SIZE - powerPelletSize) / 2, cellY + (TILE_SIZE - powerPelletSize) / 2, powerPelletSize, powerPelletSize);
                powerPellet->setBrush(powerPelletBrush); powerPellet->setPen(pelletPen);
                powerPellet->setData(0, PowerPelletType);
                addItem(powerPellet);
                m_pelletItems[QPoint(x,y)] = powerPellet;
                m_pelletCount++;
                break;
            }
            default: break;
            }
        }
    }
    m_scoreText = new QGraphicsTextItem();
    m_scoreText->setDefaultTextColor(Qt::yellow);
    m_scoreText->setFont(QFont("Arial", 16, QFont::Bold));
    m_scoreText->setPos(TILE_SIZE, -TILE_SIZE * 2);
    addItem(m_scoreText);
    m_livesText = new QGraphicsTextItem();
    m_livesText->setDefaultTextColor(Qt::yellow);
    m_livesText->setFont(QFont("Arial", 16, QFont::Bold));
    m_livesText->setPos(MAP_WIDTH * TILE_SIZE - TILE_SIZE * 6, -TILE_SIZE * 2);
    addItem(m_livesText);
}

void GameScene::clearScene()
{
    qDeleteAll(items());
    m_pacmans.clear();
    m_ghosts.clear();
    m_pelletItems.clear();
    m_scoreText = nullptr;
    m_livesText = nullptr;
}

QList<CharacterState> GameScene::getPacmanStates() const
{
    QList<CharacterState> states;
    for(PacMan* pacman : m_pacmans) {
        CharacterState s;
        s.id = pacman->getPlayerId();
        s.position = pacman->pos();
        states.append(s);
    }
    return states;
}

QList<CharacterState> GameScene::getGhostStates() const
{
    QList<CharacterState> states;
    for(Ghost* ghost : m_ghosts) {
        CharacterState s;
        s.id = ghost->getPlayerId();
        s.position = ghost->pos();
        states.append(s);
    }
    return states;
}

int GameScene::getScore() const
{
    return m_score;
}
