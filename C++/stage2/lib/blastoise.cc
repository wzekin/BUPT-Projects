#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 水箭龟
 */
class Blastoise final : public Water
{
public:
    Blastoise(int level)
        : Water(
              QUuid::createUuid(),
              QString("Blastoise"),
              QString("水箭龟"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              79, 85, 105, 78) {}

    Blastoise(const QJsonObject &json)
        : Water(
              QUuid::fromString(json["uuid"].toString()),
              QString("Blastoise"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              79, 85, 105, 78) {}
};

REGISTER_MONSTER(Blastoise, "Blastoise");
