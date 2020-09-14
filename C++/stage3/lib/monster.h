#ifndef MONSTER_H
#define MONSTER_H

#include <QObject>
#include <QUuid>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlListProperty>
#include <QVariant>
#include <iostream>
#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>
#include <cmath>

enum class Type
{
  /**
      * 水系
      */
  Water,

  /**
      * 火系
      */
  Fire,

  /**
      * 草系
      */
  Grass,

  /**
      * 飞行系
      */
  Fly
};

class Monster : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString name READ name)
  Q_PROPERTY(QString nickname READ nickname)
  Q_PROPERTY(int level READ level)
  Q_PROPERTY(int max_hp READ max_hp)
  Q_PROPERTY(int attack READ attack)
  Q_PROPERTY(int defence READ defence)
  Q_PROPERTY(int speed READ speed)
  Q_PROPERTY(QString attack_name READ attack_name)
  Q_PROPERTY(bool is_died READ is_died)
  Q_PROPERTY(bool should_level_up READ should_level_up)

public:
  /**
     * 精灵的构造函数
     */
  Monster(QUuid uuid,
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

  /**
    * 精灵的析构函数
    */
  virtual ~Monster();

public slots:
  /**
    * 精灵的攻击函数
    */
  virtual int monster_attack(Monster *other) = 0;

  /**
    * 精灵的攻击名称
    */
  virtual QString attack_name() const = 0;

  /**
    * 精灵的种类
    */
  virtual Type type() const = 0;

  /**
    * 精灵的升级函数
    * 升级时会更新各项属性，增加值取决于精灵种类和个体值
    */
  void level_up();

  /**
    * 精灵击杀了另外一个精灵调用
    * 用于更新个体值
    */
  void monster_kill(Monster *other);

  /**
   * 重置精灵的HP
   */
  void reset_hp();

  /**
    * 返回精灵的uuid
    */
  QString uuid() const;

  /**
    * 返回精灵的名字
    */
  QString name() const;

  /**
    * 返回精灵的昵称
    */
  QString nickname() const;

  /**
    * 返回精灵的等级
    */
  int level() const;

  /**
    * 返回精灵的经验
    */
  int experience() const;

  /**
    * 返回精灵的最大生命
    */
  int max_hp() const;

  /**
    * 返回精灵的攻击力
    */
  int attack() const;

  /**
    * 返回精灵的防御力
    */
  int defence() const;

  /**
    * 返回精灵的攻击速度
    */
  int speed() const;

  /**
    * 返回精灵当前的HP
    */
  int hp() const;

  /**
    * 是否处于濒死状态
    */
  bool is_died() const;

  /**
    * 是否应该升级
    */
  bool should_level_up() const;

  /**
    * 设置精灵的昵称
    */
  void set_nickname(QString &nickname);

  /**
    * 精灵被攻击
    */
  void attacked(int delta_hp);

public:
  QJsonObject toJson() const;

protected:
  /**
    * 精灵的唯一id
    */
  QUuid __uuid;

  /**
    * 精灵的名称（学名）
    */
  QString __name;

  /**
    * 精灵的昵称（玩家起名）
    */
  QString __nickname;

  /**
    * 精灵的经验
    */
  int __experience;

  /**
    * 精灵的最大HP
    */
  int __max_hp;

  /**
    * 精灵的当前HP
    */
  int __hp;

  /**
    * 精灵的攻击力
    */
  int __attack;

  /**
    * 精灵的防御力
    */
  int __defence;

  /**
    * 精灵的攻击速度
    */
  int __speed;

  /**
    * 精灵的最大HP（个体值）
    */
  int __unit_max_HP;

  /**
    * 精灵的攻击力（个体值）
    */
  int __unit_attack;

  /**
    * 精灵的防御力（个体值）
    */
  int __unit_defence;

  /**
    * 精灵的攻击速度（个体值）
    */
  int __unit_speed;

  /**
    * 精灵的最大HP（种族值）
    */
  int __species_max_HP;

  /**
    * 精灵的攻击力（种族值）
    */
  int __species_attack;

  /**
    * 精灵的防御力（种族值）
    */
  int __species_defence;

  /**
    * 精灵的攻击速度（种族值）
    */
  int __species_speed;

  /**
    * 精灵是否应该升级
    */
  bool __should_level_up;

  /**
    * 计算精灵的能力值
    */
  virtual void calc_stat() = 0;
};

#endif
