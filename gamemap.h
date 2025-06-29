// gamemap.h
#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QVector>
#include <QString>
#include <QPoint>

// Énumération pour les types de cellules de la carte
enum CellType {
    Wall = 0,         // Mur
    Pellet,           // Bille normale
    PowerPellet,      // Super Bille (power-up)
    Empty,            // Vide
    PacManSpawn1,     // Emplacement de départ du Pac-Man 1
    PacManSpawn2,     // Emplacement de départ du Pac-Man 2
    GhostSpawn1,      // Emplacement de départ du Fantôme 1
    GhostSpawn2       // Emplacement de départ du Fantôme 2
};

// Constantes pour la taille de la grille du jeu
const int TILE_SIZE = 24; // Taille d'une cellule en pixels (ajusté pour un meilleur affichage)
const int MAP_WIDTH = 28; // Nombre de cellules en largeur
const int MAP_HEIGHT = 31; // Nombre de cellules en hauteur

class GameMap
{
public:
    GameMap();

    // Retourne le type de cellule à une position donnée
    CellType cellType(int x, int y) const;
    CellType cellType(const QPoint& pos) const { return cellType(pos.x(), pos.y()); }

    // Obtenir la carte complète
    const QVector<QVector<CellType>>& getMapData() const { return m_mapData; }

    // Obtenir les positions de départ
    QPoint getPacManSpawnPoint(int id) const;
    QPoint getGhostSpawnPoint(int id) const;

private:
    QVector<QVector<CellType>> m_mapData; // La matrice représentant la carte

    // Initialise la carte avec une structure prédéfinie
    void initializeMap();
};

#endif // GAMEMAP_H
