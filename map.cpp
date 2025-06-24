#include "map.h"
#include <QDebug> // Utile pour le débogage si nécessaire

// Implémentation
PacManMap::PacManMap(QWidget *parent)
    : QWidget(parent)
    , cellSize(20)
    , playerX(1), playerY(1)
    , wallColor(Qt::blue)
    , pathColor(Qt::black)
    , pelletColor(Qt::yellow)
    , powerPelletColor(Qt::red)
    , gateColor(Qt::magenta)
    , emptyColor(Qt::darkGray)
    , playerColor(Qt::green)
{
    gameMap = createPredefinedMap();
    mapHeight = gameMap.size();
    mapWidth = gameMap.empty() ? 0 : gameMap[0].size(); // Gérer le cas d'une map vide

    setFixedSize(mapWidth * cellSize, mapHeight * cellSize);
    setFocusPolicy(Qt::StrongFocus);

    // Timer pour les animations/updates
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &PacManMap::updateGame);
    gameTimer->start(100); // 100ms
}

PacManMap::~PacManMap()
{
    // Le QTimer est un QObject enfant, donc il sera supprimé automatiquement
    // lors de la suppression de PacManMap. Pas besoin de delete gameTimer;
}


std::vector<std::vector<CellType>> PacManMap::createPredefinedMap() {
    // Dimensions : Largeur (colonnes) = 32, Hauteur (lignes) = 16
    std::vector<std::vector<CellType>> map = {
        // Ligne 0
        {Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall},
        // Ligne 1
        {Wall, PowerPellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, PowerPellet, Wall},
        // Ligne 2
        {Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall},
        // Ligne 3
        {Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall},
        // Ligne 4
        {Wall, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Wall},
        // Ligne 5
        {Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall},
        // Ligne 6 (Début zone Maison Fantômes)
        {Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Gate, Gate, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall},
        // Ligne 7 (Intérieur Maison Fantômes)
        {Gate, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Wall, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Wall, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Gate},
        // Ligne 8 (Intérieur Maison Fantômes)
        {Gate, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Wall, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Wall, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Gate},
        // Ligne 9 (Intérieur Maison Fantômes)
        {Gate, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Wall, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, Wall, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Gate},
        // Ligne 10 (Fin zone Maison Fantômes)
        {Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Pellet, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall},
        // Ligne 11
        {Wall, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Wall},
        // Ligne 12
        {Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall},
        // Ligne 13
        {Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall, Wall, Wall, Pellet, Wall, Wall, Pellet, Wall},
        // Ligne 14
        {Wall, PowerPellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, Pellet, PowerPellet, Wall},
        // Ligne 15
        {Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall, Wall}
    };
    return map;
}

// Fonctions de gestion de la map
bool PacManMap::isValidPosition(int x, int y) const {
    return x >= 0 && x < mapWidth && y >= 0 && y < mapHeight;
}

bool PacManMap::isTraversable(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    CellType type = gameMap[y][x];
    return type != Wall;
}

CellType PacManMap::getCellType(int x, int y) const {
    if (!isValidPosition(x, y)) return Wall;
    return gameMap[y][x];
}

void PacManMap::setCellType(int x, int y, CellType type) {
    if (isValidPosition(x, y)) {
        gameMap[y][x] = type;
        update(); // Demander un rafraîchissement de l'affichage
    }
}

int PacManMap::getPelletCount() const {
    int count = 0;
    for (const auto& row : gameMap) {
        for (CellType cell : row) {
            if (cell == Pellet || cell == PowerPellet) {
                count++;
            }
        }
    }
    return count;
}

std::vector<std::pair<int, int>> PacManMap::getPowerPelletPositions() const {
    std::vector<std::pair<int, int>> positions;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (gameMap[y][x] == PowerPellet) {
                positions.emplace_back(x, y);
            }
        }
    }
    return positions;
}

std::vector<std::pair<int, int>> PacManMap::getGatePositions() const {
    std::vector<std::pair<int, int>> positions;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (gameMap[y][x] == Gate) {
                positions.emplace_back(x, y);
            }
        }
    }
    return positions;
}

void PacManMap::eatPellet(int x, int y) {
    if (isValidPosition(x, y)) {
        CellType type = gameMap[y][x];
        if (type == Pellet || type == PowerPellet) {
            gameMap[y][x] = Path;
            update(); // Demander un rafraîchissement de l'affichage
        }
    }
}

std::pair<int, int> PacManMap::handleTeleport(int x, int y) const {
    // Téléportation horizontale
    if (y >= 0 && y < mapHeight) { // Assurez-vous que la ligne est valide
        if (x < 0) return {mapWidth - 1, y};
        if (x >= mapWidth) return {0, y};
    }


    // Gestion des gates spécifiques (simple: téléporte à l'autre gate si on entre sur une gate)
    if (isValidPosition(x, y) && getCellType(x, y) == Gate) {
        auto gates = getGatePositions();
        if (gates.size() >= 2) { // Il doit y avoir au moins deux gates pour la téléportation
            for (const auto& gate : gates) {
                // Si la gate n'est pas celle où l'on arrive, téléporte là
                if (gate.first != x || gate.second != y) {
                    return gate;
                }
            }
        }
    }


    return {x, y}; // Pas de téléportation si pas sur une gate ou positions invalides (hors y)
}

void PacManMap::setCellSize(int size) {
    cellSize = size;
    setFixedSize(mapWidth * cellSize, mapHeight * cellSize); // Mettre à jour la taille du widget
    update();
}


// Affichage
void PacManMap::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // Activer l'anti-aliasing

    // Dessiner la map
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            drawCell(painter, x, y, gameMap[y][x]);
        }
    }

    // Dessiner le joueur
    painter.setBrush(playerColor);
    painter.setPen(Qt::NoPen);
    int playerPixelX = playerX * cellSize + cellSize / 4;
    int playerPixelY = playerY * cellSize + cellSize / 4;
    painter.drawEllipse(playerPixelX, playerPixelY, cellSize / 2, cellSize / 2);
}

void PacManMap::drawCell(QPainter &painter, int x, int y, CellType type) {
    int pixelX = x * cellSize;
    int pixelY = y * cellSize;
    QRect cellRect(pixelX, pixelY, cellSize, cellSize);

    switch (type) {
    case Wall:
        painter.fillRect(cellRect, wallColor);
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(cellRect);
        break;

    case Path:
        painter.fillRect(cellRect, pathColor);
        break;

    case Pellet:
        painter.fillRect(cellRect, pathColor);
        painter.setBrush(pelletColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(pixelX + cellSize/3, pixelY + cellSize/3,
                            cellSize/3, cellSize/3);
        break;

    case PowerPellet:
        painter.fillRect(cellRect, pathColor);
        painter.setBrush(powerPelletColor);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(pixelX + cellSize/6, pixelY + cellSize/6,
                            2*cellSize/3, 2*cellSize/3);
        break;

    case Gate:
        painter.fillRect(cellRect, gateColor);
        painter.setPen(QPen(Qt::white, 2));
        painter.drawLine(pixelX, pixelY + cellSize/2,
                         pixelX + cellSize, pixelY + cellSize/2);
        break;

    case Empty:
        painter.fillRect(cellRect, emptyColor);
        break;
    }
}

// Contrôles
void PacManMap::keyPressEvent(QKeyEvent *event) {
    int newX = playerX;
    int newY = playerY;

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Z:
        newY--;
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        newY++;
        break;
    case Qt::Key_Left:
    case Qt::Key_Q:
        newX--;
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        newX++;
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    // Gérer d'abord la téléportation *avant* de vérifier si la nouvelle position est valide sur la map
    auto teleportPos = handleTeleport(newX, newY);
    int finalX = teleportPos.first;
    int finalY = teleportPos.second;


    // Vérifier si le mouvement est possible après téléportation (ou si pas de téléportation)
    if (isTraversable(finalX, finalY)) {
        // Manger la pastille si présente
        eatPellet(finalX, finalY);

        playerX = finalX;
        playerY = finalY;
        update(); // Demander un rafraîchissement de l'affichage

        // Vérifier si toutes les pastilles sont mangées
        if (getPelletCount() == 0) {
            qDebug() << "Jeu terminé ! Toutes les pastilles ont été mangées.";
            gameTimer->stop();
            // Ici, vous pourriez émettre un signal pour indiquer la fin du jeu
        }
    } else {
        // Si la nouvelle position (après une éventuelle téléportation) n'est pas traversable, le joueur ne bouge pas.
        // qDug("Mouvement impossible vers " << finalX << ", " << finalY);
    }
}

void PacManMap::updateGame() {
    // Ici tu peux ajouter la logique de mise à jour du jeu
    // Animation des fantômes, etc.
    // Pour l'instant, cette fonction ne fait rien dans ce code fourni.
}
