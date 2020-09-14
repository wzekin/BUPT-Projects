#include <QVariant>
#include <QVariantList>

#include "client.h"
#include "../lib/monster.h"
#include "../lib/monster_factory.h"

Client::Client(QObject* parent):QObject(parent), tcpSocket(new QTcpSocket(this)){
    in.setDevice(tcpSocket);
    tcpSocket->abort();
}

QQmlListProperty<User> Client::user_list()
{
    return QQmlListProperty<User>(this, &this->__user_list);
}

User* Client::user(){
    return this->__user;
}

Monster* Client::newMonster(User* user){
    Monster* m = MonsterFactory::random(1);
    user->monsters.push_back(m);
    this->UpsertMonster(m);
    return m;
}


QVariantList Client::signin(const QString &username, const QString &password) {
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
    if (!success) {
        list << "" << error;
    }else{
        QJsonDocument doc = QJsonDocument::fromJson(error.toUtf8());
        this->__user = new User(doc.object());
    }
    return list;
}

QVariantList Client::register_(const QString &username, const QString &password){
    tcpSocket->connectToHost("localhost", 8080);
    qDebug() << "建立链接";
    in << "register " << username << " " << password << "\n";
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);

    int success;
    QString error;
    QVariantList list;
    in >> success >> error;
    if (!success) {
        list << "" << error;
    }else {
        QJsonDocument doc = QJsonDocument::fromJson(error.toUtf8());
        this->__user = new User(doc.object());
    }
    return list;
}

void Client::listUser() {
    in << (int)SocketCode::ListUser;
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    QString json = in.readAll();
    if (success) {
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        QJsonArray array =  doc.array();
        for (auto it = __user_list.begin(); it != __user_list.end(); it++){
            delete *it;
        }
        __user_list.clear();
        for (auto it = array.begin(); it != array.end(); it++){
            User* user = new User(it->toObject());
            this->__user_list.push_back(user);
        }
    }
    return;
}

void Client::listMonster(User* user) {
    in << (int)SocketCode::ListMonster << user->uuid();
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    QString json = in.readAll();
    if (success) {
        QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
        QJsonArray array =  doc.array();
        for (auto it = user->monsters.begin(); it != user->monsters.end(); it++){
            delete *it;
        }
        user->monsters.clear();
        for (auto it = array.begin(); it != array.end(); it++){
            Monster* monster = MonsterFactory::fromJson(it->toObject());
            user->monsters.push_back(monster);
        }
    }
    return;
}


void Client::UpsertMonster(Monster* monster){
    in << (int)SocketCode::UpdateMonster << QJsonDocument(monster->toJson()).toJson();
    in.flush();
    tcpSocket->waitForBytesWritten();

    tcpSocket->waitForReadyRead(-1);
    int success;
    in >> success;
    qDebug() << success;
    return;
}
