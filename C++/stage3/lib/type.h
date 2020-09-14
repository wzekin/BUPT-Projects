#ifndef TYPE_H
#define TYPE_H
#include <map>
#include <utility>
#include <vector>
#include "monster.h"
#include "category.h"

using namespace std;

class Water : public Defensive
{
public:
    Water(QUuid uuid,
          QString name,
          QString nickname,
          int experience,
          int unit_max_HP,
          int unit_attack,
          int unit_defence,
          int unit_speed,
          int species_max_HP,
          int species_attack,
          int species_defence,
          int species_speed,
          QObject *parent = NULL)
        : Defensive(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent) {}

    Type type() const override
    {
        return Type::Water;
    }

    QString attack_name() const override
    {
        return QString("水流攻击");
    }

    int monster_attack(Monster *other) override
    {
        double bit = (double)this->attack() / (double)other->defence();
        if (other->type() == Type::Fire)
        {
            bit *= 1.5;
        }
        int damage = (double)(5 * this->level() + 10) / 10 * bit + 2;
        qDebug()
            << this->nickname()
            << "攻击了"
            << other->nickname()
            << "造成了"
            << damage
            << "点伤害";
        return damage;
    }
};

class Fire : public Strong
{
public:
    Fire(QUuid uuid,
         QString name,
         QString nickname,
         int experience,
         int unit_max_HP,
         int unit_attack,
         int unit_defence,
         int unit_speed,
         int species_max_HP,
         int species_attack,
         int species_defence,
         int species_speed,
         QObject *parent = NULL)
        : Strong(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent) {}

    Type type() const override
    {
        return Type::Fire;
    }

    QString attack_name() const override
    {
        return QString("火焰攻击");
    }

    int monster_attack(Monster *other) override
    {
        double bit = (double)this->attack() / (double)other->defence();
        if (other->type() == Type::Grass)
        {
            bit *= 1.5;
        }
        int damage = (double)(5 * this->level() + 10) / 10 * bit + 2;
        qDebug()
            << this->nickname()
            << "攻击了"
            << other->nickname()
            << "造成了"
            << damage
            << "点伤害";
        return damage;
    }
};

class Grass : public Shield
{
public:
    Grass(QUuid uuid,
          QString name,
          QString nickname,
          int experience,
          int unit_max_HP,
          int unit_attack,
          int unit_defence,
          int unit_speed,
          int species_max_HP,
          int species_attack,
          int species_defence,
          int species_speed,
          QObject *parent = NULL)
        : Shield(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent) {}
    Type type() const override
    {
        return Type::Fire;
    }

    QString attack_name() const override
    {
        return QString("树木攻击");
    }

    int monster_attack(Monster *other) override
    {
        double bit = (double)this->attack() / (double)other->defence();
        if (other->type() == Type::Water)
        {
            bit *= 1.5;
        }
        int damage = (double)(5 * this->level() + 10) / 10 * bit + 2;
        qDebug()
            << this->nickname()
            << "攻击了"
            << other->nickname()
            << "造成了"
            << damage
            << "点伤害";
        return damage;
    }
};

class Fly : public Sharp
{
public:
    Fly(QUuid uuid,
        QString name,
        QString nickname,
        int experience,
        int unit_max_HP,
        int unit_attack,
        int unit_defence,
        int unit_speed,
        int species_max_HP,
        int species_attack,
        int species_defence,
        int species_speed,
        QObject *parent = NULL)
        : Sharp(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent) {}
    Type type() const override
    {
        return Type::Fly;
    }

    QString attack_name() const override
    {
        return QString("翅膀攻击");
    }

    int monster_attack(Monster *other) override
    {
        double bit = (double)this->attack() / (double)other->defence();
        int damage = (double)(5 * this->level() + 10) / 10 * bit + 2;
        qDebug()
            << this->nickname()
            << "攻击了"
            << other->nickname()
            << "造成了"
            << damage
            << "点伤害";
        return damage;
    }
};
#endif
