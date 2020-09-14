#include "tcpserver.h"

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* tcpSocket = new QTcpSocket;
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << tcpSocket->error();
        return;
    }

    QString op, username, password;
    QTextStream* in = new QTextStream(tcpSocket);

    tcpSocket->waitForReadyRead(-1);
    *in >> op >> username >> password;
    try{
        if (op == "login"){
            User* user = this->db().login(username,password);
            QJsonDocument doc =  QJsonDocument(user->toJsonObject());

            *in << true << doc.toJson();
            in->flush();
            tcpSocket->waitForBytesWritten();

            this->__list.push(user);
            UserThread *thread = new UserThread(tcpSocket, in, user, this);
            connect(thread, &UserThread::finished, thread, &UserThread::deleteLater);
            thread->start();
        }else if (op == "register"){
            User* user = this->db().register_(username,password);
            QJsonDocument doc =  QJsonDocument(user->toJsonObject());

            *in << true << doc.toJson();
            in->flush();
            tcpSocket->waitForBytesWritten();

            UserThread *thread = new UserThread(tcpSocket, in, user, this);
            connect(thread, &UserThread::finished, thread, &UserThread::deleteLater);
            thread->start();
        }else {
            throw std::exception("错误的输入");
        }
    } catch(std::exception e) {
        qDebug() << e.what();
        *in << false << e.what();
        in->flush();
        tcpSocket->waitForBytesWritten();
        tcpSocket->disconnectFromHost();
        return;
    }
}

UserThread::UserThread(QTcpSocket* socket, QTextStream* stream, User *user, TcpServer *parent):
    QThread(parent),socket(socket), stream(stream), user(user), db(DB(user->uuid())) {
}

void UserThread::run(){
    qDebug() << "start run";
     while(true){
        if (!this->socket->waitForReadyRead(-1)){
            break;
        }
        int tag;
        *this->stream >> tag;
        switch (tag) {
            case (int)SocketCode::ListUser: {
                QJsonDocument jsonData = QJsonDocument(((TcpServer*)this->parent())->list().getJson());
                qDebug() << jsonData;
                *this->stream << true << jsonData.toJson();
                this->stream->flush();
                this->socket->waitForBytesWritten();
                break;
            };
            case (int)SocketCode::ListMonster: {
                QString uuid;
                *this->stream >> uuid;
                qDebug() <<uuid;
                try{
                    QJsonDocument jsonData = QJsonDocument(this->db.listMonster(uuid));
                    *this->stream << true << jsonData.toJson();
                    this->stream->flush();
                    this->socket->waitForBytesWritten();
                } catch(std::exception e) {
                    qDebug() << e.what();
                    *this->stream << false << e.what();
                    this->stream->flush();
                    this->socket->waitForBytesWritten();
                }
                break;
            }
            case (int)SocketCode::UpdateMonster: {
                QString rawData = this->stream->readAll();
                QJsonDocument jsonData = QJsonDocument::fromJson(rawData.toUtf8());
                qDebug() <<jsonData;
                try{
                    this->db.upsertMonster(jsonData.object(), this->user);
                    *this->stream << true;
                    this->stream->flush();
                    this->socket->waitForBytesWritten();
                } catch(std::exception e) {
                    *this->stream << false << e.what();
                    this->stream->flush();
                    this->socket->waitForBytesWritten();
                }
                break;
            }
            case (int)SocketCode::ReleaseMonster: {
                QString uuid;
                *this->stream >> uuid;
                qDebug() <<uuid;
                try{
                    this->db.releaseMonster(uuid);
                    *this->stream << true;
                    this->stream->flush();
                    this->socket->waitForBytesWritten();
                } catch(std::exception e) {
                    qDebug() << e.what();
                    *this->stream << false << e.what();
                    this->stream->flush();
                    this->socket->waitForBytesWritten();
                }
                break;
            }
        }
     }
     this->finish();
}

void UserThread::finish(){
    qDebug() << this->user->username() << "已登出";
    ((TcpServer*)this->parent())->list().del(this->user);
}
