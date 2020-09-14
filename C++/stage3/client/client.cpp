#include <QVariant>
#include <QVariantList>

#include "client.h"
#include "../lib/monster.h"
#include "../lib/monster_factory.h"

Client::Client(QObject *parent) : QObject(parent), tcpSocket(new QTcpSocket(this))
{
    in.setDevice(tcpSocket);
    tcpSocket->abort();
}

QQmlListProperty<User> Client::user_list()
{
    return QQmlListProperty<User>(this, &this->__user_list);
}

QQmlListProperty<Monster> Client::monster_list()
{
    return QQmlListProperty<Monster>(this, &this->__random_monsters);
}

User *Client::user()
{
    return this->__user;
}

Monster *Client::newMonster()
{
    Monster *m = MonsterFactory::random(1);
    this->__user->monsters.push_back(m);
    this->upsertMonster(m);
    return m;
}

void Client::addMonster(Monster *monster)
{
    Monster *m = MonsterFactory::produce(monster->name(), 1);
    this->__user->monsters.push_back(m);
    this->upsertMonster(m);
}

void Client::random9(int max_level)
{
    for (auto const &it : this->__random_monsters)
        delete it;
    this->__random_monsters.clear();
    for (int i = 0; i < 9; i++)
    {
        int level = rand() % (max_level + 1) + 1;
        this->__random_monsters.push_back(MonsterFactory::random(level));
    }
}

QVariantList Client::signin(const QString &username, const QString &password)
{
    tcpSocket->connectToHost("localhost", 8080);
    qDebug() << "建立链接";
    in << "login " << username << " " << password << "\n";
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);

    int success;
    QVariantList list;
    in >> success;
    QString error = in.readAll();
    if (!success)
    {
        list << "" << error;
    }
    else
    {
        QJsonDocument doc = QJsonDocument::fromJson(error.toUtf8());
        this->__user = new User(doc.object());
        qDebug() << error;
    }
    return list;
}

QVariantList Client::register_(const QString &username, const QString &password)
{
    tcpSocket->connectToHost("localhost", 8080);
    qDebug() << "建立链接";
    in << "register " << username << " " << password << "\n";
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);

    int success;
    QVariantList list;
    in >> success;
    QString error = in.readAll();
    if (!success)
    {
        list << "" << error;
    }
    else
    {
        QJsonDocument doc = QJsonDocument::fromJson(error.toUtf8());
        this->__user = new User(doc.object());
    }
    return list;
}

void Client::listUser()
{
    in << (int)SocketCode::ListUser;
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    QString json = in.readAll();
    if (success)
    {
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        QJsonArray array = doc.array();
        for (auto it = __user_list.begin(); it != __user_list.end(); it++)
        {
            delete *it;
        }
        __user_list.clear();
        for (auto it = array.begin(); it != array.end(); it++)
        {
            User *user = new User(it->toObject());
            this->listMonster(user);
            this->__user_list.push_back(user);
        }
    }
    return;
}

void Client::listMonster(User *user)
{
    in << (int)SocketCode::ListMonster << user->uuid();
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    QString json = in.readAll();
    if (success)
    {
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        QJsonArray array = doc.array();
        for (auto it = user->monsters.begin(); it != user->monsters.end(); it++)
        {
            delete *it;
        }
        user->monsters.clear();
        for (auto it = array.begin(); it != array.end(); it++)
        {
            Monster *monster = MonsterFactory::fromJson(it->toObject());
            user->monsters.push_back(monster);
        }
    }
    return;
}

void Client::upsertMonster(Monster *monster)
{
    QJsonDocument doc = QJsonDocument(monster->toJson());
    in << (int)SocketCode::UpdateMonster << doc.toJson();
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    QString error = in.readAll();
    qDebug() << error;
    return;
}

void Client::releaseMonster(Monster *monster)
{
    in << (int)SocketCode::ReleaseMonster << monster->uuid();
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    QString error = in.readAll();
    qDebug() << error;
    return;
}

QVariantList Client::random3()
{
    QVariantList l;
    int len = this->__user->monsters.length();
    if (len <= 3)
    {
        for (auto const &a : this->__user->monsters)
        {
            l << QVariant::fromValue(a);
        }
    }
    else
    {
        l << QVariant::fromValue(this->__user->monsters[0]);
        l << QVariant::fromValue(this->__user->monsters[len - 1]);
        l << QVariant::fromValue(this->__user->monsters[len / 2]);
    }
    return l;
}

void Client::addBattle(bool is_win)
{
    in << (int)SocketCode::AddBattle << (is_win ? "success" : "fail");
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    QString error = in.readAll();
    qDebug() << error;
    return;
}
