#include "db.h"

DB::DB(QString name)
{
    this->__db = QSqlDatabase::addDatabase("QSQLITE",name);
    this->__db.setHostName("myServer");
    this->__db.setDatabaseName("./data.db");
    Q_ASSERT(this->__db.open());
}

User* DB::login(QString username, QString password){
    QSqlQuery query(this->__db);
    query.prepare("SELECT uuid, battle_count, win_count from \"user\" where username = ? and password = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();
    if (query.lastError().type() != QSqlError::NoError){
        throw std::exception(query.lastError().text().toUtf8());
    }
    if (query.next()){
        QUuid uuid(query.value(0).toString());
        return new User(uuid, username, query.value(1).toInt(), query.value(2).toInt());
    }else{
        throw std::exception("用户名或密码错误");
    }
}

User* DB::register_(QString username, QString password){
    QSqlQuery query(this->__db);
    query.prepare("insert into \"user\" (uuid, username, password) values (?,?,?)");
    User* user = new User(QUuid::createUuid(), username);
    query.addBindValue(user->uuid());
    query.addBindValue(username);
    query.addBindValue(password);
    query.exec();
    if (query.lastError().type() != QSqlError::NoError){
        throw std::exception(query.lastError().text().toUtf8());
    }
    Monster* monster = MonsterFactory::random(1);
    this->upsertMonster(monster->toJson(),user);
    monster = MonsterFactory::random(1);
    this->upsertMonster(monster->toJson(),user);
    monster = MonsterFactory::random(1);
    this->upsertMonster(monster->toJson(),user);
    return user;
}

QJsonArray DB::listMonster(QString uuid){
    QJsonArray array;
    QSqlQuery query(this->__db);
    query.prepare("select * from monster where master = ? and is_released = false");
    query.addBindValue(uuid);
    query.exec();
    if (query.lastError().type() != QSqlError::NoError){
        throw std::exception(query.lastError().text().toUtf8());
    }
    while(query.next()){
        array.push_back(QJsonObject{
                           {"uuid", query.value(0).toString()},
                           {"name", query.value(1).toString()},
                           {"nickname", query.value(2).toString()},
                           {"experience", query.value(3).toInt()},
                           {"unit_max_hp", query.value(4).toInt()},
                           {"unit_attack", query.value(5).toInt()},
                           {"unit_defence", query.value(6).toInt()},
                           {"unit_speed", query.value(7).toInt()},
                        });
    }
    return array;
}

void DB::upsertMonster(QJsonObject json, User* master){
    QSqlQuery query(this->__db);
    query.prepare("INSERT OR REPLACE INTO monster (uuid, name, nickname, experience, unit_max_hp, unit_attack, unit_defence, unit_speed, master) VALUES (?,?,?,?,?,?,?,?,?)");
    qDebug() << json["name"].toString();
    query.addBindValue(json["uuid"].toString());
    query.addBindValue(json["name"].toString());
    query.addBindValue(json["nickname"].toString());
    query.addBindValue(json["experience"].toInt());
    query.addBindValue(json["unit_max_hp"].toInt());
    query.addBindValue(json["unit_attack"].toInt());
    query.addBindValue(json["unit_defence"].toInt());
    query.addBindValue(json["unit_speed"].toInt());
    query.addBindValue(master->uuid());
    query.exec();
    if (query.lastError().type() != QSqlError::NoError){
        throw std::exception(query.lastError().text().toUtf8());
    }
}

void DB::releaseMonster(QString uuid){
    QSqlQuery query(this->__db);
    query.prepare("UPDATE monster set is_released = true where uuid = ?");
    query.addBindValue(uuid);
    query.exec();
    if (query.lastError().type() != QSqlError::NoError){
        throw std::exception(query.lastError().text().toUtf8());
    }
}

void DB::addBattle(User *user, bool is_win){
    QSqlQuery query(this->__db);
    user->addBattle(is_win);
    query.prepare("UPDATE user set battle_count = ?, win_count = ? where uuid = ?");
    query.addBindValue(user->battle_count());
    query.addBindValue(user->win_count());
    query.addBindValue(user->uuid());
    query.exec();
    if (query.lastError().type() != QSqlError::NoError){
        throw std::exception(query.lastError().text().toUtf8());
    }
}
