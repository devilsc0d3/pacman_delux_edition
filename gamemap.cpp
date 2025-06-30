// gamemap.cpp
#include "gamemap.h"
#include <QDebug>

GameMap::GameMap()
{
    initializeMap();
}

void GameMap::initializeMap()
{
    // 'P' = PacManSpawn1, 'Q' = PacManSpawn2
    QString mapString[MAP_HEIGHT] = {
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
        "W............WW............W",
        "W.WWWW.WW.WW.WW.WW.WW.WWWW.W",
        "W...oW.W...W.WW.W...W.Wo...W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W....P.............Q.......W", // P et Q (Pac-Man)
        "W.WWWW.WW.WW.WW.WW.WW.WWWW.W",
        "W.W.WW.WW.WW.WW.WW.WW.WW/W.W",
        "W......WW....WW....WW......W",
        "WWWWWW.WW.WWWWWWWW.WW.WWWWWW",
        "W..........................W",
        "W..WWWWW..WWWWWW..WWWWWW...W",  // Début C P P alignés
        "W..WW.....WW  WW..WW  WW...W",  // C P P
        "W..WW.....WW  WW..WW  WW...W",  // C P P
        "W..WW.....WWWWWW..WWWWWW...W",  // C P P (barres horizontales des P)
        "W..WW.....WW......WW.......W",  // C P P
        "W..WW.....WW......WW.......W",  // C P P
        "W..WWWWW..WW......WW.......W",  // Fin C P P
        "W..........................W",
        "WWWWWW.WW.WWWWWWWW.WW.WWWWWW",
        "WWWWWW.WW.WWWWWWWW.WW.WWWWWW", // Maison des fantômes
        "W......WW..W    W..WW......W",
        "W.WWW.WW...WG H  ...WW.WWW.W", // Fantômes dans la maison
        "W.....WWWW.W    W.WWWW.....W",
        "WWWWW.WWWW.WWWWWW.WWWW.WWWWW",
        "W..........................W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W...oW.......WW.......Wo...W",
        "W.WWWW.WW.WWWWWWWW.WW.WWWW.W",
        "W..........................W",
        "WWWWWWWWWWWWWWWWWWWWWWWWWWWW"
    };

    m_mapData.resize(MAP_HEIGHT);
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        m_mapData[y].resize(MAP_WIDTH);
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (x >= mapString[y].length()) {
                m_mapData[y][x] = Empty;
                continue;
            }
            switch (mapString[y][x].toLatin1()) {
            case 'W': m_mapData[y][x] = Wall; break;
            case '.': m_mapData[y][x] = Pellet; break;
            case 'o': m_mapData[y][x] = PowerPellet; break;
            case ' ': m_mapData[y][x] = Empty; break;
            case 'P': m_mapData[y][x] = PacManSpawn1; break;
            case 'Q': m_mapData[y][x] = PacManSpawn2; break;
            case 'G': m_mapData[y][x] = GhostSpawn1; break;
            case 'H': m_mapData[y][x] = GhostSpawn2; break;
            default: m_mapData[y][x] = Empty; break;
            }
        }
    }
}

CellType GameMap::cellType(int x, int y) const
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
        return Wall;
    }
    return m_mapData[y][x];
}

QPoint GameMap::getPacManSpawnPoint(int id) const
{
    CellType spawnType = (id == 0) ? PacManSpawn1 : PacManSpawn2;
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (m_mapData[y][x] == spawnType) {
                return QPoint(x, y);
            }
        }
    }
    return QPoint(1, 1);
}

QPoint GameMap::getGhostSpawnPoint(int id) const
{
    CellType spawnType = (id == 0) ? GhostSpawn1 : GhostSpawn2;
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (m_mapData[y][x] == spawnType) {
                return QPoint(x, y);
            }
        }
    }
    return QPoint(MAP_WIDTH / 2, MAP_HEIGHT / 2);
}
