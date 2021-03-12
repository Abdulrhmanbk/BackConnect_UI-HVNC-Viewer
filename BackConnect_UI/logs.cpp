#include "logs.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
logs::logs(QWidget *parent) : QWidget(parent)
{
    QTableWidget *logs = new QTableWidget;
    logs->setSelectionBehavior(QAbstractItemView::SelectRows);
    logs->setSelectionMode(QAbstractItemView::SingleSelection);
    logs->setFixedHeight(350);
    logs->setFixedWidth(950);
    QStringList headers;
    headers << "Type" << "Date" << "Log Data";

    logs->setColumnCount(3); logs->setHorizontalHeaderLabels(headers);
    for(int i = 0 ; i < 3; i++) logs->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);

    logs->setShowGrid(false);
    logs->setWordWrap(true);
    logs->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    logs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QVBoxLayout *vl2 = new QVBoxLayout;
    QHBoxLayout *hl1 = new QHBoxLayout;
    vl2->addLayout(hl1);
    vl2->addSpacing(10); vl2->addWidget(logs);
    vl2->addStretch();
    this->setLayout(vl2);

}
