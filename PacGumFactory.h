#ifndef PACGUMFACTORY_H
#define PACGUMFACTORY_H

#include <QObject>
#include <QSharedPointer>
#include <QVector>

class PacGum;
class Bonus;
class Map;

enum class BonusGenerationStrategy {
    BonusOnCorner,
    BonusRandomize
};

struct GumEntity {
    QSharedPointer<PacGum> gum;
    int x;
    int y;
};

class PacGumFactory : public QObject
{
    Q_OBJECT

public:
    explicit PacGumFactory(QObject* parent = nullptr);

    QVector<GumEntity> populateMap(const Map& map, BonusGenerationStrategy strategy);

private:
    bool isCorner(const Map& map, int x, int y) const;
};

#endif // PACGUMFACTORY_H
