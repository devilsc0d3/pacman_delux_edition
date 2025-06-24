#include "MapView.h"

// Constructor: Takes a reference to the Map data object
MapView::MapView(const Map& map, QWidget *parent)
    : QWidget(parent)
    , m_map(map) // Store the reference to the map data
    , m_cellSize(20) // Default cell size
    , m_wallColor(Qt::blue)
    , m_pathColor(Qt::black)
    , m_pelletColor(Qt::yellow)
    , m_powerPelletColor(Qt::red)
    , m_gateColor(Qt::magenta)
    , m_emptyColor(Qt::darkGray)
{
    // Set the fixed size of the widget based on the map dimensions and cell size.
    // Use the map.cols() and map.rows() methods from the Map data object.
    setFixedSize(m_map.cols() * m_cellSize, m_map.rows() * m_cellSize);

    // Note: Removed game timer, player position, input handling.
    // This class is only for drawing the map grid.
}

// Set the size of each cell in pixels
void MapView::setCellSize(int size) {
    m_cellSize = size;
    // Update the widget size if the cell size changes
    setFixedSize(m_map.cols() * m_cellSize, m_map.rows() * m_cellSize);
    update(); // Request a repaint
}

// paintEvent: Called by Qt whenever the widget needs to be redrawn
void MapView::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event) // Indicate that the event parameter is not used

    QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true); // Optional: anti-aliasing

    // Iterate through each cell of the Map data object and draw it.
    for (int y = 0; y < m_map.rows(); ++y) {
        for (int x = 0; x < m_map.cols(); ++x) {
            // Use the getCellType method from the Map data object
            CellType type = m_map.getCellType(x, y);
            drawCell(painter, x, y, type);
        }
    }

    // Note: Player drawing logic would go here or in a separate layer.
}

// drawCell: Helper method to draw a single cell at grid coordinates (x, y)
void MapView::drawCell(QPainter &painter, int x, int y, CellType type) {
    int pixelX = x * m_cellSize;
    int pixelY = y * m_cellSize;
    QRect cellRect(pixelX, pixelY, m_cellSize, m_cellSize);

    // Draw the background first
    QColor backgroundColor = m_pathColor; // Default to path color
    if (type == CellType::Wall) backgroundColor = m_wallColor;
    else if (type == CellType::Empty) backgroundColor = m_emptyColor;
    else if (type == CellType::Gate) backgroundColor = m_gateColor; // Gates might have unique background/line drawing

    painter.fillRect(cellRect, backgroundColor);

    // Draw specific elements for Pellets and PowerPellets over the path background
    painter.setPen(Qt::NoPen); // Don't draw outlines for pellets/powerpellets

    switch (type) {
    case CellType::Wall:
        // Optional: Draw wall outline
        painter.setPen(QPen(Qt::white, 1));
        painter.drawRect(cellRect);
        break;
    case CellType::Pellet:
        painter.setBrush(m_pelletColor);
        painter.drawEllipse(pixelX + m_cellSize/3, pixelY + m_cellSize/3,
                            m_cellSize/3, m_cellSize/3);
        break;
    case CellType::PowerPellet:
        painter.setBrush(m_powerPelletColor);
        painter.drawEllipse(pixelX + m_cellSize/6, pixelY + m_cellSize/6,
                            2*m_cellSize/3, 2*m_cellSize/3);
        break;
    case CellType::Gate:
        // Optional: Draw a line for the gate
        painter.setPen(QPen(Qt::white, 2)); // White line, 2 pixels thick
        painter.drawLine(pixelX, pixelY + m_cellSize/2,
                         pixelX + m_cellSize, pixelY + m_cellSize/2);
        break;
    case CellType::Path:
    case CellType::Empty:
        // Background already drawn
        break;
    }
}

// Note: keyPressEvent, updateGame and other game logic methods are not included
// as they belong in a game state manager, not the map view.
