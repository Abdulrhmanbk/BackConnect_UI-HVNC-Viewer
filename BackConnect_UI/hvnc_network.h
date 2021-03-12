#ifndef HVNC_NETWORK_H
#define HVNC_NETWORK_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

class hvnc_network : public QTcpServer
{
public:
    hvnc_network(QObject *parent = 0);
};

#endif // HVNC_NETWORK_H
