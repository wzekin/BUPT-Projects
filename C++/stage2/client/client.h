#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QTcpSocket>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QQmlListProperty>

#include "../lib/user.h"
#include "../lib/monster.h"
#include "../lib/socket.h"


class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject* parent = nullptr);
    Q_PROPERTY(QQmlListProperty<User> user_list READ user_list)
    Q_PROPERTY(User* user READ user)

public slots:
    QVariantList signin(const QString &username, const QString &password);
    QVariantList register_(const QString &username, const QString &password);
    void listUser();
    void listMonster(User* user);
    void UpsertMonster(Monster* monster);

    Monster* newMonster(User* user);
//    QVariantList changePassword(const QString &new_password);
//    QVariantList updateOrInsertMonster(const Monster& monster);
//    QVariantList listMonsters();
public:
    QQmlListProperty<User> user_list();
    QQmlListProperty<Monster> monster_list();
    User* user();

private:
    QTcpSocket *tcpSocket;
    QTextStream in;

    User* __user;
    QList<User*> __user_list;
};

#endif // CLIENT_H
