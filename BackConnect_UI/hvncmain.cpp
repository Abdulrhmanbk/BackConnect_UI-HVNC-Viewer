#include "hvncmain.h"
#include "hvnc_main_view.h"
#include "backconnect.h"
#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>
#include <QLabel>
#include <cstring>
#define PORT 420
void hvncmain::onReadyRead()
{
// removed
}
hvncmain::hvncmain(QWidget *parent, QTcpSocket *tcpSocket[1]) : QWidget(parent)
{

    this->tcpDesktop = tcpSocket[DESKTOP];
    this->tcpSocket = tcpSocket[USER_INPUT];

    connect(this->tcpDesktop ,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    QHBoxLayout *hl = new QHBoxLayout();
    QVBoxLayout *vl = new QVBoxLayout();
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height() /2;
    int width = screenGeometry.width() / 2;
    pixmap = QPixmap(height, width);
    pixmap.fill(Qt::black);
    label = new QLabel();
    label->setPixmap(pixmap);
    label->setAlignment(Qt::AlignCenter);
    hl->addWidget(label);
    vl->addLayout(hl);
    this->setLayout(vl);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMouseTracking(true);
    label->setMouseTracking(true);
}



void hvncmain::paintEvent(QPaintEvent *event){
 //   qDebug("paint event");

}
WPARAM hvncmain::wParamMouseAction(Qt::MouseButtons MouseButton){
    WPARAM wParam = Qt::NoButton;
    switch(MouseButton){
    case Qt::ExtraButton17:
        //MK_CONTROL
        wParam |= MK_CONTROL;
    break;
    case Qt::ExtraButton14:
        //MK_LBUTTON
        wParam |= MK_LBUTTON;
        break;
    case Qt::ExtraButton18:
        wParam |= MK_MBUTTON;
        break;
    case Qt::ExtraButton15:
        wParam |= MK_RBUTTON;
        break;
    case Qt::ExtraButton16:
        wParam |= MK_SHIFT;
        break;
    case Qt::ExtraButton19:
        wParam |= MK_XBUTTON1;
        break;
    case Qt::ExtraButton20:
        wParam |= MK_XBUTTON2;
        break;
    default:
        break;
    }
    return wParam;
}
void hvncmain::mouseDoubleClickEvent(QMouseEvent *event){
    LPARAM lpParam = MAKELPARAM(event->x(), event->y());
    int msg = 0;
    switch(event->buttons()){
    case Qt::LeftButton:
        msg = WM_LBUTTONDBLCLK;
        break;
    case Qt::RightButton:
        msg = WM_RBUTTONDBLCLK;
        break;
    case Qt::MidButton:
        msg = WM_MBUTTONDBLCLK;
        break;
    default:
        return;
    }
    WPARAM wParam = wParamMouseAction(event->buttons());
    hvnc_main_view::send_event(tcpSocket, msg, wParam, lpParam);
    // handle wParam
}
void hvncmain::mousePressEvent(QMouseEvent *event){
// there may be a ratio here needed since we might not be the same size as the otehr crap.
    if(event->type() == QEvent::MouseButtonDblClick)
        return;

    LPARAM lpParam = MAKELPARAM(event->x(), event->y());
    int msg = 0;
    switch(event->buttons()){
    case Qt::LeftButton:
       // add support for wParam.
        msg = WM_LBUTTONDOWN;
        break;
    case Qt::RightButton:
        msg = WM_RBUTTONDOWN;
        break;
    case Qt::MidButton:
        msg = WM_MBUTTONDOWN;
        break;

    default:
        return;
    }
    WPARAM wParam = wParamMouseAction(event->buttons());

    hvnc_main_view::send_event(tcpSocket, msg, wParam, lpParam);
    // setup wParam since look at what this shit requires WM_LBUTTONDOWN etc

}
void hvncmain::mouseReleaseEvent(QMouseEvent *event){
    if(event->type() == QEvent::MouseButtonDblClick)
        return;

    LPARAM lpParam = MAKELPARAM(event->x(), event->y());
    int msg = 0;
    switch(event->buttons()){
    case Qt::LeftButton:
        msg = WM_LBUTTONUP;
        break;
    case Qt::RightButton:
        msg = WM_RBUTTONUP;
        break;
    case Qt::MidButton:
        msg = WM_MBUTTONUP;
        break;
    default:
        break;
    }
    WPARAM wParam = wParamMouseAction(event->buttons());

    hvnc_main_view::send_event(tcpSocket, msg, wParam, lpParam);
    // setup wParam
}
void hvncmain::mouseMoveEvent(QMouseEvent *event){
    if(label->hasMouseTracking()){
        qDebug("event->x %d\nevent->y %d\n", event->x(), event->y());
        int x_pos = event->x();
        int y_pos = event->y();
        int msg = WM_MOUSEMOVE;
        LPARAM lpParam = MAKELPARAM(x_pos,y_pos);
        WPARAM wParam = wParamMouseAction(event->buttons());
        hvnc_main_view::send_event(tcpSocket, msg, wParam, lpParam);
    }

}
void hvncmain::wheelEvent(QWheelEvent *event){
    qDebug("event->x %d\nevent->y %d\n", event->x(), event->y());
    int x_pos = event->x();
    int y_pos = event->y();
    int msg = WM_MOUSEMOVE;
    LPARAM lpParam = MAKELPARAM(x_pos, y_pos);
    int distance = event->angleDelta().y() / WHEEL_DELTA;
    WPARAM wParamAction = wParamMouseAction(event->buttons());
    WORD l = LOWORD(wParamAction);
    WPARAM wParam = MAKEWPARAM(l, distance);
    hvnc_main_view::send_event(tcpSocket, msg, wParam, lpParam);

}
void hvncmain::keyReleaseEvent(QKeyEvent *event){
    qDebug("key release event:\n\ttext: %s\n\tvkey: %x", event->text().toStdString().c_str(), event->nativeVirtualKey());
    WPARAM wParam = event->nativeVirtualKey();
       //lParam???
    hvnc_main_view::send_event(tcpSocket, WM_KEYUP, wParam, 0);

}
void hvncmain::keyPressEvent(QKeyEvent *event){
    qDebug("key press event:\n\ttext: %s\n\tvkey: %x", event->text().toStdString().c_str(), event->nativeVirtualKey());
    WPARAM wParam = event->nativeVirtualKey();
    //lParam???
    hvnc_main_view::send_event(tcpSocket, WM_KEYDOWN, wParam, 0);
    wParam = event->key(); // this is what WM_CHAR needs right?
       //lParam???
    hvnc_main_view::send_event(tcpSocket, WM_CHAR, wParam, 0);
}
