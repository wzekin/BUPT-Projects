#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 天然鸟
 */
class Xatu final : public Fly
{
public:
    Xatu(int level)
        : Fly(
              QUuid::createUuid(),
              QString("Xatu"),
              QString("天然鸟"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              65, 95, 70, 100) {}

    Xatu(const QJsonObject &json)
        : Fly(
              QUuid::fromString(json["uuid"].toString()),
              QString("Xatu"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              65, 95, 70, 100) {}
};

REGISTER_MONSTER(Xatu, "Xatu");
