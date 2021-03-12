#ifndef HVNCMAIN_H
#define HVNCMAIN_H
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>
#include <windows.h>
#include <QMap>
#include <QGuiApplication.h>
#include <QScreen.h>

class hvncmain : public QWidget
{
    Q_OBJECT
    QTcpSocket *tcpSocket;
    QTcpSocket *tcpDesktop;
    QPoint topLeft;
    QPoint bottomRight;
    QRect MainRect;
    QPixmap pixmap;
    QPainter *painter;
    QBrush brush;
    QLabel *label;
    int my_width;
    int my_height;


    void layout_init();
    QWidget *status_widget;
    hvncmain *vncmain;
public:
    //QTcpS *tcpServer;

//    hvncmain(QWidget *parent = nullptr);

    void onReadyRead();
    QByteArray fullBitmap;
    bool configured = false;
    int width = 0, height = 0, dwBmpSize = 0, dwSizeofDIB = 0;
    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;
    WPARAM wParamMouseAction(Qt::MouseButtons MouseButton);
    hvncmain(QWidget *parent, QTcpSocket *tcpSocket[1]);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void wheelEvent(QWheelEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);
private:

};
// hvnc crap, identifier which is the peerName nigga which can be gotten from QTcpSocket
#endif // HVNCVIEWER_H
