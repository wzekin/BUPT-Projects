#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QUuid>
#include <QObject>
#include <QString>
#include <QVector>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>
#include <QJsonDocument>
#include <QtGlobal>
#include <memory>
#include "../lib/user.h"
#include "../lib/socket.h"
#include "userlist.h"
#include "db.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer(QObject *parent = nullptr):QTcpServer(parent){};
    UserList& list(){
        return this->__list;
    }
    DB& db() {
        return this->__db;
    };


protected:
    void incomingConnection(qintptr socketDescriptor) override;

    UserList __list;
    DB __db;
};
#endif // TCPSERVER_H



class UserThread : public QThread
{
    Q_OBJECT

public:
    UserThread(QTcpSocket* socket, QTextStream* stream, User *user, TcpServer *parent);

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

public Q_SLOTS:
    void finish();

private:
    QTcpSocket* socket;
    QTextStream* stream;
    User *user;
    DB db;
};
