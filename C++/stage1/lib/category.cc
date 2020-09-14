#include "category.h"

Strong::Strong(QUuid uuid,
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
               QObject *parent)
    : Monster(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent)
{
    calc_stat();
    this->__hp = this->__max_hp;
}

void Strong::calc_stat()
{
    this->__max_hp = (this->__species_max_HP + this->__unit_max_HP) *
                         this->level() * 6 / 50 +
                     10 + this->level() * 6;
    this->__attack = (this->__species_attack + this->__unit_attack) *
                         this->level() * 8 / 50 +
                     5;
    this->__defence = (this->__species_defence + this->__unit_defence) *
                          this->level() * 6 / 50 +
                      5;
    this->__speed = (this->__species_speed + this->__unit_speed) *
                        this->level() * 6 / 50 +
                    5;
}

Shield::Shield(QUuid uuid,
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
               QObject *parent)
    : Monster(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent)
{
    calc_stat();
    this->__hp = this->__max_hp;
}

void Shield::calc_stat()
{
    this->__max_hp = (this->__species_max_HP + this->__unit_max_HP) *
                         this->level() * 8 / 50 +
                     10 + this->level() * 6;
    this->__attack = (this->__species_attack + this->__unit_attack) *
                         this->level() * 6 / 50 +
                     5;
    this->__defence = (this->__species_defence + this->__unit_defence) *
                          this->level() * 6 / 50 +
                      5;
    this->__speed = (this->__species_speed + this->__unit_speed) *
                        this->level() * 6 / 50 +
                    5;
}

Defensive::Defensive(QUuid uuid,
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
                     QObject *parent)
    : Monster(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent)
{

    calc_stat();
    this->__hp = this->__max_hp;
}

void Defensive::calc_stat()
{
    this->__max_hp = (this->__species_max_HP + this->__unit_max_HP) *
                         this->level() * 8 / 50 +
                     10 + this->level() * 6;
    this->__attack = (this->__species_attack + this->__unit_attack) *
                         this->level() * 6 / 50 +
                     5;
    this->__defence = (this->__species_defence + this->__unit_defence) *
                          this->level() * 8 / 50 +
                      5;
    this->__speed = (this->__species_speed + this->__unit_speed) *
                        this->level() * 6 / 50 +
                    5;
}

Sharp::Sharp(QUuid uuid,
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
             QObject *parent)
    : Monster(uuid, name, nickname, experience, unit_max_HP, unit_attack, unit_defence, unit_speed, species_max_HP, species_attack, species_defence, species_speed, parent)
{
    calc_stat();
    this->__hp = this->__max_hp;
}

void Sharp::calc_stat()
{
    this->__max_hp = (this->__species_max_HP + this->__unit_max_HP) *
                         this->level() * 8 / 50 +
                     10 + this->level() * 6;
    this->__attack = (this->__species_attack + this->__unit_attack) *
                         this->level() * 6 / 50 +
                     5;
    this->__defence = (this->__species_defence + this->__unit_defence) *
                          this->level() * 6 / 50 +
                      5;
    this->__speed = (this->__species_speed + this->__unit_speed) *
                        this->level() * 8 / 50 +
                    5;
}