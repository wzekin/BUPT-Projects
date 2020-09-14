#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include "../lib/user.h"
#include "../lib/monster_factory.h"


/**
 * 数据库类，用于与数据库交互
 */
class DB
{
public:
    DB(QString name = QSqlDatabase::defaultConnection);
    User* login(QString username, QString password);
    User* register_(QString username, QString password);
    QJsonArray listMonster(QString uuid);
    void upsertMonster(QJsonObject json, User* master);
    void releaseMonster(QString uuid);
    void addBattle(User *user, bool is_win);
private:
    QSqlDatabase __db;
};

#endif // DB_H
