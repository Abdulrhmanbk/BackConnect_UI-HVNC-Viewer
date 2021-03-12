#include "hvnc_main_view.h"
#include "backconnect.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>


hvnc_main_view::hvnc_main_view(QWidget *parent, QTcpSocket *tcpSocket[1]) : QWidget(parent, Qt::Window)
{
    this->tcpSocket = tcpSocket[USER_INPUT];
    layout_init(tcpSocket);
    connections();
}

void hvnc_main_view::layout_init(QTcpSocket *tcpSocket[1]){
  QHBoxLayout *hl = new QHBoxLayout();
  QVBoxLayout *vl = new QVBoxLayout();
  vncmain = new hvncmain(this, tcpSocket);

  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  int height = (screenGeometry.height() /2);
  int width = (screenGeometry.width() / 2);
  vncmain->setFixedWidth(width);
  vncmain->setFixedHeight(height);
  hl->addWidget(vncmain);
  vl->addLayout(hl);
  centralWidget = new QWidget();
  button_ScreenShot = new QPushButton("Screen Shot");
  checkbox_Control = new QCheckBox("Control Screen");
  checkbox_Keyboard = new QCheckBox("Keyboard Input");
  button_Disconnect = new QPushButton("Disconnect");
  button_FireFox = new QPushButton("Start Firefox");
  button_Chrome = new QPushButton("Start Chrome");
  button_Edge = new QPushButton("Start Edge");
  button_InternetExplorer = new QPushButton("Start Internet Explorer");
  button_Opera = new QPushButton("Start Opera");
  button_Execute = new QPushButton("Execute Command");
  commands = new QCheckBox("Enable/Disable command");
  textEdit = new QLineEdit ("Enter Command:");
  layout = new QGridLayout ();
  centralWidget->setLayout (layout);
  layout->addWidget (checkbox_Control, 1, 0);
  layout->addWidget (checkbox_Keyboard, 1, 1);
  layout->addWidget (button_ScreenShot, 2, 0);
  layout->addWidget (button_Disconnect, 2, 1);
  layout->addWidget (button_FireFox, 2, 2);
  layout->addWidget (button_Chrome, 2, 3);
  layout->addWidget (button_Edge, 2, 4);
  layout->addWidget (button_InternetExplorer, 2, 5);
  layout->addWidget (button_Opera, 2, 6);
  layout->addWidget(commands, 3, 0);
  layout->addWidget(textEdit, 3, 1);
  layout->addWidget(button_Execute, 3, 2);
  centralWidget->setFixedHeight(100);
  vl->addWidget(centralWidget);

  setLayout(vl);
  //this->setCentralWidget(window);
}
void hvnc_main_view::commands_button(int state){
    if(state == Qt::Checked){
       textEdit->setReadOnly(false);
       textEdit->clear();
    }
     else
       textEdit->setReadOnly(true);
}
void hvnc_main_view::send_event(QTcpSocket *tcpSocket, int msg, WPARAM wParam, LPARAM lpParam){
    tcpSocket->write((const char*)&msg, sizeof(msg));
    tcpSocket->write((const char*)&wParam, sizeof(wParam));
    tcpSocket->write((const char*)&lpParam, sizeof(lpParam));
}
void hvnc_main_view::execute_button(){

    send_event(tcpSocket, COMMAND, 0, 0);
    const char *command = textEdit->text().toStdString().c_str();
    int commandlen = textEdit->text().length();
    tcpSocket->write(command, commandlen);
}

void hvnc_main_view::chrome_button(){
    send_event(tcpSocket, CHROME, 0, 0);
}
void hvnc_main_view::disconnect_button(){

}
void hvnc_main_view::edge_button(){
    send_event(tcpSocket, EDGE, 0, 0);
}
void hvnc_main_view::firefox_button(){

    send_event(tcpSocket, FIREFOX, 0, 0);
}
void hvnc_main_view::internet_explorer_button(){

    send_event(tcpSocket, IE, 0, 0);
}
void hvnc_main_view::opera_button(){

    send_event(tcpSocket, OPERA, 0, 0);
}
void hvnc_main_view::screenshot_button(){

}
void hvnc_main_view::connections()
{
    connect(button_Execute, SIGNAL(clicked(bool)), this, SLOT(execute_button()));
    connect(button_Chrome, SIGNAL(clicked(bool)), this, SLOT(chrome_button()));
    connect(button_Disconnect, SIGNAL(clicked(bool)), this, SLOT(disconnect_button()));
    connect(button_Edge, SIGNAL(clicked(bool)), this, SLOT(edge_button()));
    connect(button_FireFox, SIGNAL(clicked(bool)), this, SLOT(firefox_button()));
    connect(button_InternetExplorer, SIGNAL(clicked(bool)), this, SLOT(internet_explorer_button()));
    connect(button_Opera, SIGNAL(clicked(bool)), this, SLOT(opera_button()));
    connect(button_ScreenShot, SIGNAL(clicked(bool)), this, SLOT(screenshot_button()));
    connect(commands, SIGNAL(stateChanged(int)), this, SLOT(commands_button(int)));
}
