
#include "logs.h"
#include "backconnect.h"
#include "main_controller.h"

main_controller::main_controller(QWidget *parent ) : QMainWindow(parent){
    QListWidgetItem *list_item[3];
    sideBar = new QListWidget();;
    const char* links[] = {"Clients Connected", "Logs"};
    sideBar->setSizeAdjustPolicy(QListWidget::AdjustToContents);

    for(size_t i = 0; i < 2; i++)
    {
        list_item[i] = new QListWidgetItem(links[i], sideBar);
        list_item[i]->setSizeHint(QSize(list_item[i]->sizeHint().width(), 80 ));
        list_item[i]->setTextAlignment(Qt::AlignCenter);

    }

    window_stack = new QStackedWidget();
    backconnect *bck = new backconnect();
    logs *log = new logs();

    window_stack->addWidget(bck);
    window_stack->addWidget(log);


    QHBoxLayout *hl[3]; QVBoxLayout *vl[3];
    for(size_t i = 0; i < 3; i++) {
        hl[i] = new QHBoxLayout();
        vl[i] = new QVBoxLayout();
    }

    //For Sidebar
    hl[0]->addSpacing(-9);
    vl[1]->addWidget(sideBar);
    hl[0]->addLayout(vl[1]);
    vl[2]->addWidget(window_stack);
    hl[0]->addSpacing(-6);
    hl[0]->addLayout(vl[2]);
    hl[0]->addSpacing(-9);
    vl[0]->addLayout(hl[1]); vl[0]->addSpacing(-7); vl[0]->addLayout(hl[0]);
    vl[0]->addSpacing(-8);
    sideBar->setFixedWidth(180);
    QWidget *window = new QWidget();

    window->setLayout(vl[0]);
    this->setCentralWidget(window);
    sideBar->setItemSelected(list_item[0] , true);
    connect(sideBar , SIGNAL(currentRowChanged(int)) , this , SLOT(sideBarIndexChanged(int)));
}

main_controller::~main_controller(){

}


void main_controller :: sideBarIndexChanged(int idx)
{
    if(sideBar->currentRow() != idx){
        sideBar->setCurrentRow(idx);
        return;
    }
    if(window_stack->widget(idx) != nullptr)
            window_stack->setCurrentIndex(idx);
}
