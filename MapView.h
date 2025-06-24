#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include "map.h" // Inclure la classe Map de données

// Forward declaration (optional but good practice)
// class Map;

class MapView : public QWidget
{
    Q_OBJECT // QWidget require the Q_OBJECT macro

public:
    // The MapView needs a reference to the Map data object to draw it.
    explicit MapView(const Map& map, QWidget *parent = nullptr);
    // Destructor is default as members are managed by Qt or std::
    ~MapView() = default;

    // Set the size in pixels for each cell on the display.
    void setCellSize(int size);
    // Get the current cell size.
    int getCellSize() const { return m_cellSize; }

    // Note: We don't include game logic or player position here.
    // This widget's job is just to display the map grid.
    // Player, pellets, etc. would typically be drawn either on top
    // in this widget's paintEvent, or by a separate widget/layer.
    // For simplicity, this version just draws the base map tiles.

protected:
    // The core method for drawing the widget's contents.
    void paintEvent(QPaintEvent *event) override;

private:
    // Helper method to draw a single cell based on its type.
    void drawCell(QPainter &painter, int x, int y, CellType type);

    // Reference to the Map data object to be displayed.
    const Map& m_map;
    // Size of each cell in pixels.
    int m_cellSize;

    // Colors for different cell types.
    QColor m_wallColor;
    QColor m_pathColor;
    QColor m_pelletColor;
    QColor m_powerPelletColor;
    QColor m_gateColor;
    QColor m_emptyColor;
    // Note: Removed player color as player isn't drawn by this widget.
};

#endif // MAPVIEW_H
