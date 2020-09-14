#ifndef USERLIST_H
#define USERLIST_H

#include "../lib/user.h"
#include <QObject>
#include <QUuid>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

/**
 *  用于在后端存储User， 加锁防止数据冲突
 */
class UserList
{
public:
    // 拿到实例
    QVector<User *> &get();

    /**
     * 转换为json格式
     */
    QJsonArray getJson() const;

    /**
     * 添加数据
     */
    void push(User *u);

    /**
     * 删除数据
     */
    void del(const User *u);

    /**
     * 清理数据
     */
    void clear();

private:
    QVector<User *> __users;

    //读写锁
    mutable std::shared_mutex __mutex;
};

#endif // USERLIST_H