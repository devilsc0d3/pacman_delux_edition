#include "map.h"
// Note: QDebug is removed as it's not needed for the map data itself.
// Other Qt headers like QPainter, QKeyEvent, QTimer are removed.

// Constructor: Initializes the map data
Map::Map()
{
    m_gameMap = createPredefinedMap();
    m_mapHeight = m_gameMap.size();
    m_mapWidth = m_gameMap.empty() ? 0 : m_gameMap[0].size(); // Handle empty map case
}

// Helper function to define the map layout
std::vector<std::vector<CellType>> Map::createPredefinedMap() {
    // This is the same predefined map data you provided, now used
    // to initialize the m_gameMap member.
    std::vector<std::vector<CellType>> map = {
        // Ligne 0
        {CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall},
        // Ligne 1
        {CellType::PowerPellet, CellType::PowerPellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::PowerPellet, CellType::Wall}, // Correction: Added Wall at end
        // Ligne 2
        {CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall},
        // Ligne 3
        {CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall},
        // Ligne 4
        {CellType::Wall, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Wall},
        // Ligne 5
        {CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall},
        // Ligne 6 (Début zone Maison Fantômes)
        {CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Gate, CellType::Gate, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall},
        // Ligne 7 (Intérieur Maison Fantômes - Note: Pac-Man usually can't enter Empty/Gate)
        {CellType::Gate, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Wall, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Wall, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Gate},
        // Ligne 8 (Intérieur Maison Fantômes)
        {CellType::Gate, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Wall, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Wall, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Gate},
        // Ligne 9 (Intérieur Maison Fantômes)
        {CellType::Gate, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Wall, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Empty, CellType::Wall, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Gate},
        // Ligne 10 (Fin zone Maison Fantômes)
        {CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall},
        // Ligne 11
        {CellType::Wall, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Wall},
        // Ligne 12
        {CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall},
        // Ligne 13
        {CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall, CellType::Wall, CellType::Pellet, CellType::Wall},
        // Ligne 14
        {CellType::Wall, CellType::PowerPellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::Pellet, CellType::PowerPellet, CellType::Wall},
        // Ligne 15
        {CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall, CellType::Wall}
    };
    return map;
}

// Get the number of rows (height)
int Map::rows() const {
    return m_mapHeight;
}

// Get the number of columns (width)
int Map::cols() const {
    return m_mapWidth;
}

// Check if coordinates are within the map boundaries
bool Map::isValidPosition(int x, int y) const {
    return x >= 0 && x < m_mapWidth && y >= 0 && y < m_mapHeight;
}

// Get the type of cell at coordinates, returns Wall for invalid positions
CellType Map::getCellType(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return CellType::Wall; // Treat out-of-bounds as Wall
    }
    return m_gameMap[y][x];
}


// Check if a cell is walkable (not a Wall)
bool Map::isWalkable(int x, int y) const {
    // isWall includes out-of-bounds check
    return getCellType(x, y) != CellType::Wall;
}

// Note: Removed functions related to game state, display, and interaction:
// setCellType
// getPelletCount
// getPowerPelletPositions
// getGatePositions
// eatPellet
// handleTeleport
// setCellSize
// getCellSize
// paintEvent
// drawCell
// keyPressEvent
// updateGame
