#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 大食花
 */
class Victreebel final : public Grass
{
public:
    Victreebel(int level)
        : Grass(
              QUuid::createUuid(),
              QString("Victreebel"),
              QString("大食花"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              80, 105, 70, 70) {}

    Victreebel(const QJsonObject &json)
        : Grass(
              QUuid::fromString(json["uuid"].toString()),
              QString("Victreebel"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              80, 105, 70, 70) {}
};

REGISTER_MONSTER(Victreebel, "Victreebel");
