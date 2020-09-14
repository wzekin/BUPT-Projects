#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 喷火龙
 */
class Charizard final : public Fire
{
public:
    Charizard(int level)
        : Fire(
              QUuid::createUuid(),
              QString("Charizard"),
              QString("喷火龙"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              78, 109, 85, 100) {}

    Charizard(const QJsonObject &json)
        : Fire(
              QUuid::fromString(json["uuid"].toString()),
              QString("Charizard"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              78, 109, 85, 100) {}
};

REGISTER_MONSTER(Charizard, "Charizard");
