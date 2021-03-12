#ifndef BACKCONNECT_REAL_H
#define BACKCONNECT_REAL_H
#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
class main_controller : public QMainWindow
{
    Q_OBJECT
public:
    QListWidget *sideBar;
    QStackedWidget *window_stack;
    explicit main_controller(QWidget *parent = nullptr);
    ~main_controller();
signals:

public slots:
private slots:
     void sideBarIndexChanged(int);
};

#endif // BACKCONNECT_REAL_H
