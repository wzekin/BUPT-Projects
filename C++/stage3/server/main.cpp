#include "tcpserver.h"
#include <QHostAddress>

int main(){
    TcpServer *tcpServer = new TcpServer();
    if (!tcpServer->listen(QHostAddress::Any,8080)) {
        return 0;
    }
    while(true){
        if(tcpServer->waitForNewConnection(10000)){
        }
    }
}
