#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 大比鸟
 */
class Pidgeot final : public Fly
{
public:
    Pidgeot(int level)
        : Fly(
              QUuid::createUuid(),
              QString("Pidgeot"),
              QString("大比鸟"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              83, 80, 75, 110) {}

    Pidgeot(const QJsonObject &json)
        : Fly(
              QUuid::fromString(json["uuid"].toString()),
              QString("Pidgeot"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              83, 80, 75, 110) {}
};

REGISTER_MONSTER(Pidgeot, "Pidgeot");
