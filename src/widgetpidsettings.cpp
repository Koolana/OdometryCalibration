#include "include/widgetpidsettings.h"

#include <QDebug>

WidgetPIDSettings::WidgetPIDSettings()
{
    this->setTitle("PID settings");

    this->globalLayout = new QGridLayout();

    auto lP = new QLabel("P :");
    this->globalLayout->addWidget(lP, 0, 0);

    this->leP = new QLineEdit("0.005");
    this->globalLayout->addWidget(this->leP, 0, 1);

    auto lI = new QLabel("I :");
    this->globalLayout->addWidget(lI, 1, 0);

    this->leI = new QLineEdit("7");
    this->globalLayout->addWidget(this->leI, 1, 1);

    auto lD = new QLabel("D :");
    this->globalLayout->addWidget(lD, 2, 0);

    this->leD = new QLineEdit("0.005");
    this->globalLayout->addWidget(this->leD, 2, 1);

    this->btnApply = new QPushButton("Apply");
    connect(this->btnApply, SIGNAL(clicked()), this, SLOT(sendPID()));
    this->globalLayout->addWidget(this->btnApply, 3, 0, 1, 2);

    this->globalLayout->setRowStretch(4, 1);

    this->setLayout(this->globalLayout);
}

void WidgetPIDSettings::setPID(const PID& data) {
    this->leP->setText(QString::number(data.p));
    this->leI->setText(QString::number(data.i));
    this->leD->setText(QString::number(data.d));

    this->update();

    sendPID();
}

void WidgetPIDSettings::sendPID() {
    PID data;
    data.p = this->leP->text().toFloat();
    data.i = this->leI->text().toFloat();
    data.d = this->leD->text().toFloat();

    emit changedPID(data);
}
