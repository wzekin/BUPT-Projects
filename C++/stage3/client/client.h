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

enum class SocketCode
{
    ListUser = 1,
    ListMonster = 2,
    AddBattle = 3,
    UpdateMonster = 4,
    ReleaseMonster = 5,
};

/**
 * 通信类，用于与数据库和qml端进行通信
 */
class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject *parent = nullptr);
    Q_PROPERTY(QQmlListProperty<User> user_list READ user_list)
    Q_PROPERTY(QQmlListProperty<Monster> monsters READ monster_list)
    Q_PROPERTY(User *user READ user)

public slots:
    /**
      * 登录
      */
    QVariantList signin(const QString &username, const QString &password);

    /**
     * 注册
     */
    QVariantList register_(const QString &username, const QString &password);

    /**
     * 列出所有在线用户
     */
    void listUser();

    /**
     * 列出用户所有宝可梦
     */
    void listMonster(User *user);

    /**
     * 更新宝可梦数据
     */
    void upsertMonster(Monster *monster);

    /**
     * 放生宝可梦
     */
    void releaseMonster(Monster *monster);

    /**
     * 增加宝可梦
     */
    void addMonster(Monster *monster);

    /**
     * 随机列出9个宝可梦
     */
    void random9(int level);

    /**
     * 增加一次battle记录
     */
    void addBattle(bool is_win);

    /**
     * 随机用户的三个宝可梦
     */
    QVariantList random3();

    /**
     * 为用户随机新增一个宝可梦
     */
    Monster *newMonster();

public:
    QQmlListProperty<User> user_list();
    QQmlListProperty<Monster> monster_list();
    User *user();

private:
    QTcpSocket *tcpSocket;
    QTextStream in;

    User *__user;
    QList<User *> __user_list;

    QList<Monster *> __random_monsters;
};

#endif // CLIENT_H
