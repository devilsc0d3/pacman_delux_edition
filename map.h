#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <vector>
#include <utility> // Pour std::pair

enum CellType {
    Wall,
    Path,
    Pellet,
    Empty,
    PowerPellet,
    Gate
};

class PacManMap : public QWidget
{
    Q_OBJECT

public:
    explicit PacManMap(QWidget *parent = nullptr);
    ~PacManMap(); // Déclaration du destructeur

    // Fonctions de gestion de la map
    bool isValidPosition(int x, int y) const;
    bool isTraversable(int x, int y) const;
    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);

    // Fonctions de jeu
    int getPelletCount() const;
    std::vector<std::pair<int, int>> getPowerPelletPositions() const;
    std::vector<std::pair<int, int>> getGatePositions() const;
    void eatPellet(int x, int y);

    // Téléportation
    std::pair<int, int> handleTeleport(int x, int y) const;

    // Dimensions
    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }

    // Taille des cellules pour l'affichage
    void setCellSize(int size);
    int getCellSize() const { return cellSize; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:
    std::vector<std::vector<CellType>> createPredefinedMap();
    void drawCell(QPainter &painter, int x, int y, CellType type);

    std::vector<std::vector<CellType>> gameMap;
    int mapWidth;
    int mapHeight;
    int cellSize;

    // Position du joueur pour démonstration
    int playerX, playerY;
    QTimer *gameTimer;

    // Couleurs
    QColor wallColor;
    QColor pathColor;
    QColor pelletColor;
    QColor powerPelletColor;
    QColor gateColor;
    QColor emptyColor;
    QColor playerColor;
};

#endif // MAP_H
