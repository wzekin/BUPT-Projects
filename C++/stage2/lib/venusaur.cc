#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 妙蛙花
 */
class Venusaur final : public Grass
{
public:
    Venusaur(int level)
        : Grass(
              QUuid::createUuid(),
              QString("Venusaur"),
              QString("妙蛙花"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              80, 100, 100, 80) {}

    Venusaur(const QJsonObject &json)
        : Grass(
              QUuid::fromString(json["uuid"].toString()),
              QString("Venusaur"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              80, 100, 100, 80) {}
};

REGISTER_MONSTER(Venusaur, "Venusaur");
