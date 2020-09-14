#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 九尾
 */
class Ninetales final : public Fire
{
public:
    Ninetales(int level)
        : Fire(
              QUuid::createUuid(),
              QString("Ninetales"),
              QString("九尾"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              73, 100, 100, 100) {}

    Ninetales(const QJsonObject &json)
        : Fire(
              QUuid::fromString(json["uuid"].toString()),
              QString("Ninetales"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              73, 100, 100, 100) {}
};

REGISTER_MONSTER(Ninetales, "Ninetales");
