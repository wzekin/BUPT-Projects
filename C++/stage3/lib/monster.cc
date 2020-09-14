#include "monster.h"
#include <cmath>
#include <cstdlib>
#include <QtCore>
#include <QDebug>

Monster::Monster(
        QUuid uuid,
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
        QObject* parent
        )
      :QObject(parent),
      __uuid(uuid),
      __name(name),
      __nickname(nickname),
      __experience(experience),
      __unit_max_HP(unit_max_HP),
      __unit_attack(unit_attack),
      __unit_defence(unit_defence),
      __unit_speed(unit_speed),
      __species_max_HP(species_max_HP),
      __species_attack(species_attack),
      __species_defence(species_defence),
      __species_speed(species_speed),
      __should_level_up(false)
{
}

Monster::~Monster(){}

void Monster::level_up() {
    calc_stat();
    this->__hp = this->__max_hp;
    this->__should_level_up = false;
}

void Monster::monster_kill(Monster* other) {
    int old_level = this->level();
    this->__experience  += 30;
    if (old_level < this->level()){
        this->__should_level_up = true;
    }
}

void Monster::attacked(int delta_hp){
    this->__hp -= delta_hp;
}

void Monster::reset_hp(){
    this->__hp = this->__max_hp;
}

QString Monster::uuid() const {
    return this->__uuid.toString();
}

QString Monster::name() const {
    return this->__name;
}

QString Monster::nickname() const {
    return this->__nickname;
}

int Monster::level() const {
    return std::min(this->__experience / 30, 15);
}

int Monster::experience() const {
    return this->__experience;
}

int Monster::max_hp() const {
    return this->__max_hp;
}

int Monster::attack() const {
    return this->__attack;
}

int Monster::defence() const {
    return this->__defence;
}

int Monster::speed() const {
    return this->__speed;
}

int Monster::hp() const {
    return this->__hp;
}

bool Monster::is_died() const {
    return this->__hp <= 0;
}

bool Monster::should_level_up() const {
    return this->__should_level_up;
}

void Monster::set_nickname(QString& nickname) {
    this->__nickname = nickname;
}

//void Monster::calc_stat() {
//    this->__max_hp = (this->__species_max_HP() + this->__unit_max_HP) *
//            this->level() * 6 / 50 +
//            10 + this->level() * 6;
//    this->__attack = (this->__species_attack() + this->__unit_attack) *
//            this->level() * 6 / 50 +
//            5;
//    this->__defence = (this->__species_defence() + this->__unit_defence) *
//            this->level() * 6 / 50 +
//            5;
//    this->__speed = (this->__species_speed() + this->__unit_speed) *
//            this->level() * 6 / 50 +
//            5;
//}

QJsonObject Monster::toJson() const {
    return QJsonObject{
       {"uuid", __uuid.toString()},
       {"name", __name},
       {"nickname", __nickname},
       {"experience", __experience},
       {"unit_max_hp", __unit_max_HP},
       {"unit_attack", __unit_attack},
       {"unit_defence", __unit_defence},
       {"unit_speed", __unit_speed},
    };
}


void qml_register_types_Monster()
{
    qmlRegisterTypesAndRevisions<Monster>("Monster", 1);
    qmlRegisterModule("Monster", 1, 0);
}

static const QQmlModuleRegistration registration("Monster", 1, qml_register_types_Monster);
