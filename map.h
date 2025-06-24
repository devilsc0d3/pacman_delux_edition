#ifndef MAP_H
#define MAP_H

#include <vector>
// Note: QObject, QWidget, QPainter, QKeyEvent, QTimer are removed
// as this class is no longer a GUI widget.
// std::pair is not needed for the factory's requirements.

// Define the types of cells that can exist on the map
enum class CellType { // Using enum class for stronger typing
    Wall,
    Path,
    Pellet,       // Original PacGum location
    Empty,        // Inside ghost house
    PowerPellet,  // Original Bonus location
    Gate          // Ghost house gate
};

// A simple class to represent the game map structure.
// It stores the layout and provides methods to query cell properties.
class Map
{
public:
    // Constructor loads or creates the map data.
    explicit Map();
    // Default destructor is sufficient as members are managed automatically.
    ~Map() = default;

    // --- Required by PacGumFactory ---
    // Get the number of rows (height) in the map.
    int rows() const;
    // Get the number of columns (width) in the map.
    int cols() const;
    // Check if a cell at the given coordinates is traversable by entities
    // (i.e., not a Wall). Also handles out-of-bounds gracefully.
    bool isWalkable(int x, int y) const;

    // --- Potentially useful for other parts, but not required by Factory ---
    // Check if coordinates are within the map boundaries.
    bool isValidPosition(int x, int y) const;
    // Get the type of cell at the given coordinates. Returns Wall for invalid positions.
    CellType getCellType(int x, int y) const;

private:
    // Helper function to create the predefined map layout.
    std::vector<std::vector<CellType>> createPredefinedMap();

    // The main data structure for the map.
    std::vector<std::vector<CellType>> m_gameMap;
    // Dimensions of the map.
    int m_mapWidth;
    int m_mapHeight;

    // Note: Removed members related to WIDGET, PAINTING, PLAYER, TIMER, GAME LOGIC:
    // int cellSize;
    // int playerX, playerY;
    // QTimer *gameTimer;
    // QColor wallColor, pathColor, etc.
};

#endif // MAP_H
