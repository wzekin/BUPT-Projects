#include "user.h"

User::User(const QJsonObject &json)
{
    this->__uuid = QUuid::fromString(json["uuid"].toString());
    this->__username = json["username"].toString();
    this->__battle_count = json["battle_count"].toInt();
    this->__win_count = json["win_count"].toInt();
}

QString User::uuid() const
{
    return __uuid.toString();
}

/**
     * 返回用户姓名
     */
QString User::username() const
{
    return __username;
}

/**
     * 返回用户进行决斗赛的次数
     */
int User::battle_count() const
{
    return __battle_count;
}

/**
     * 返回用户赢得决斗赛的次数
     */
int User::win_count() const
{
    return __win_count;
}

/**
     * 返回用户胜率
     */
int User::win_rate() const
{
    if (battle_count() == 0)
    {
        return 0;
    }
    return (int)((double)win_count() / (double)battle_count() * 100);
}

/**
     * 判断用户是否有金牌
     */
bool User::has_good() const
{
    int index = 0;
    for (auto const &m : this->monsters)
    {
        if (m->level() == 15)
        {
            index++;
        }
    }
    return index >= 5;
}

/**
     * 判断用户是否有银牌
     */
bool User::has_silver() const
{
    return this->monsters.length() >= 5;
}

/**
     * 返回用户的宝可梦列表（for qml
     */
QQmlListProperty<Monster> User::monster_list()
{
    return QQmlListProperty<Monster>(this, &this->monsters);
}

QJsonObject User::toJsonObject() const
{
    return QJsonObject{
        {"uuid", __uuid.toString()},
        {"username", __username},
        {"battle_count", __battle_count},
        {"win_count", __win_count}};
}

void qml_register_types_User()
{
    qmlRegisterTypesAndRevisions<User>("User", 1);
    qmlRegisterModule("User", 1, 0);
}

static const QQmlModuleRegistration registration("User", 1, qml_register_types_User);
