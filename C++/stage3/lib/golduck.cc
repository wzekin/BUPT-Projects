#include <QUuid>
#include <QJsonObject>
#include "monster_factory.h"
#include "type.h"

/**
 * 哥达鸭
 */
class Goldluck final : public Water
{
public:
    Goldluck(int level)
        : Water(
              QUuid::createUuid(),
              QString("Goldluck"),
              QString("哥达鸭"),
              level * 30,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              rand() % 32,
              80, 95, 80, 85) {}

    Goldluck(const QJsonObject &json)
        : Water(
              QUuid::fromString(json["uuid"].toString()),
              QString("Goldluck"),
              json["nickname"].toString(),
              json["experience"].toInt(),
              json["unit_max_hp"].toInt(),
              json["unit_attack"].toInt(),
              json["unit_defence"].toInt(),
              json["unit_speed"].toInt(),
              80, 95, 80, 85) {}
};

REGISTER_MONSTER(Goldluck, "Goldluck");
