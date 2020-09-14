#ifndef CATEGORY_H
#define CATEGORY_H

#include <QUuid>
#include <QString>
#include <QObject>
#include "monster.h"

class Strong : public Monster
{
public:
    Strong(QUuid uuid,
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
           QObject *parent = nullptr);

protected:
    void calc_stat() override;
};

class Shield : public Monster
{
public:
    Shield(QUuid uuid,
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
           QObject *parent = nullptr);

protected:
    void calc_stat() override;
};

class Defensive : public Monster
{
public:
    Defensive(QUuid uuid,
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
              QObject *parent = nullptr);

protected:
    void calc_stat() override;
};

class Sharp : public Monster
{
public:
    Sharp(QUuid uuid,
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
          QObject *parent = nullptr);

protected:
    void calc_stat() override;
};

#endif // CATEGORY_H
