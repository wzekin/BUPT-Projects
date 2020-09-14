#ifndef USER_H
#define USER_H

#include <QObject>
#include <QUuid>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVariant>
#include <shared_mutex>
#include <memory>
#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>
#include "monster.h"

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uuid READ uuid)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(int win_rate READ win_rate)
    Q_PROPERTY(bool has_good READ has_good)
    Q_PROPERTY(bool has_silver READ has_silver)
    Q_PROPERTY(QQmlListProperty<Monster> monsters READ monster_list)

public:
    User(QUuid uuid = QUuid(), QString username = "", int battle_count = 0, int win_count = 0, QObject *parent = nullptr) : QObject(parent), __uuid(uuid), __username(username), __battle_count(battle_count), __win_count(win_count){};
    User(const User &user) : QObject(user.parent()), __uuid(user.__uuid), __username(user.__username), __win_count(user.__win_count), __battle_count(user.__battle_count){};
    User(const QJsonObject &json);

    /**
     * 返回uuid
     */
    QString uuid() const;

    /**
     * 返回用户姓名
     */
    QString username() const;

    /**
     * 返回用户进行决斗赛的次数
     */
    int battle_count() const;

    /**
     * 返回用户赢得决斗赛的次数
     */
    int win_count() const;

    /**
     * 返回用户胜率
     */
    int win_rate() const;

    /**
     * 判断用户是否有金牌
     */
    bool has_good() const;

    /**
     * 判断用户是否有银牌
     */
    bool has_silver() const;


    /**
     * 返回用户的宝可梦列表（for qml
     */
    QQmlListProperty<Monster> monster_list();

    /**
     * 生成json数据
     */
    QJsonObject toJsonObject() const;

    /**
     * 用户的宝可梦列表
     */
    QList<Monster *> monsters;

    /**
     * 增加一次战斗数据
     */
    void addBattle(bool is_win)
    {
        this->__battle_count += 1;
        is_win ? this->__win_count += 1 : 0;
    }

private:
    /**
     * 用户UUID
     */
    QUuid __uuid;

    /**
     * 用户名称
     */
    QString __username;

    /**
     * 决斗赛次数
     */
    int __battle_count;

    /**
     * 决斗赛胜利次数
     */
    int __win_count;
};

Q_DECLARE_METATYPE(User)
#endif // USER_H