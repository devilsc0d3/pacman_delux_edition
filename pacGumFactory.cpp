#include "PacGumFactory.h"
#include "pacGum.h"
#include "Bonus.h"
#include "map.h" // Ton objet Map

#include <QRandomGenerator>

PacGumFactory::PacGumFactory(QObject* parent)
    : QObject(parent)
{
}

QVector<GumEntity> PacGumFactory::populateMap(const Map& map, BonusGenerationStrategy strategy)
{
    QVector<GumEntity> result;

    int rows = map.rows();
    int cols = map.cols();

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (!map.isWalkable(x, y))
                continue;

            bool isBonus = false;

            if (strategy == BonusGenerationStrategy::BonusOnCorner && isCorner(map, x, y)) {
                isBonus = true;
            } else if (strategy == BonusGenerationStrategy::BonusRandomize &&
                       QRandomGenerator::global()->bounded(100) < 10) {
                isBonus = true;
            }

            QSharedPointer<PacGum> gum;
            if (isBonus)
                gum = QSharedPointer<Bonus>::create();
            else
                gum = QSharedPointer<PacGum>::create();

            result.append({ gum, x, y });
        }
    }

    return result;
}

bool PacGumFactory::isCorner(const Map& map, int x, int y) const
{
    int walls = 0;
    if (!map.isWalkable(x - 1, y)) ++walls;
    if (!map.isWalkable(x + 1, y)) ++walls;
    if (!map.isWalkable(x, y - 1)) ++walls;
    if (!map.isWalkable(x, y + 1)) ++walls;

    return walls >= 2;
}
