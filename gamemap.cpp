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
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "WoWWWW.WWWWW.WW.WWWWW.WWWWoW",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W..........................W",
        "W.WWWW.WW.WWWWWW.WW.WWWW.W",
        "W.WWWW.WW.WWWWWW.WW.WWWW.W",
        "W......WW....WW....WW......W",
        "WWWWWW.WWWWW.WW.WWWWW.WWWWWW",
        "     W.WWWWW.WW.WWWWW.W     ",
        "     W.WW         WW.W     ",
        "     W.WW WWWWWWW WW.W     ",
        "WWWWWW.WW W G H W WW.WWWWWW", // Fantômes G, H
        "        . W   W .        ", // Maison des fantômes vide de Pac-Man
        "WWWWWW.WW W   W W WW.WWWWWW", // Retrait de Q ici
        "     W.WW WWWWWWW WW.W     ",
        "     W.WW         WW.W     ",
        "WWWWWW.WW.WWWWWWW.WW.WWWWWW",
        "W............WW............W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "WoWWWW.WWWWW.WW.WWWWW.WWWWoW",
        "W...WW................WW...W",
        "WWW.WW.WW.WWWWWW.WW.WW.WWW",
        "WWW.WW.WW.WWWWWW.WW.WW.WWW",
        "W....P.........Q...........W", // <-- LIGNE MODIFIÉE : P et Q sur la même ligne
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W.WWWW.WWWWW.WW.WWWWW.WWWW.W",
        "W............WW............W",
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
