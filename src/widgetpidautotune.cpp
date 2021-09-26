#include "include/widgetpidautotune.h"

#include <QDebug>

WidgetPidAutotune::WidgetPidAutotune()
{
    this->setTitle("Autotune");

    this->globalLayout = new QGridLayout();

    this->lAccuracyIndicate = new QLabel(QString::number(0.00, 'f', 2));
    this->lAccuracyIndicate->setAlignment(Qt::AlignCenter);
    this->globalLayout->addWidget(this->lAccuracyIndicate, 0, 0, 4, 2);

    auto lP = new QLabel("P :");
    this->globalLayout->addWidget(lP, 5, 0);

    this->lPValue = new QLabel(QString::number(0.00, 'f', 2));
    this->globalLayout->addWidget(this->lPValue, 5, 1);

    auto lI = new QLabel("I :");
    this->globalLayout->addWidget(lI, 6, 0);

    this->lIValue = new QLabel(QString::number(0.00, 'f', 2));
    this->globalLayout->addWidget(this->lIValue, 6, 1);

    auto lD = new QLabel("D :");
    this->globalLayout->addWidget(lD, 7, 0);

    this->lDValue = new QLabel(QString::number(0.00, 'f', 2));
    this->globalLayout->addWidget(this->lDValue, 7, 1);

    this->btnTune = new QPushButton("Tune");
    connect(this->btnTune, SIGNAL(clicked()), this, SIGNAL(tune()));
    this->globalLayout->addWidget(this->btnTune, 8, 0, 1, 2);

    this->btnAccept = new QPushButton("Accept");
    connect(this->btnAccept, SIGNAL(clicked()), this, SLOT(accept()));
    this->globalLayout->addWidget(this->btnAccept, 9, 0, 1, 2);

    this->globalLayout->setRowStretch(10, 1);

    this->setLayout(this->globalLayout);
}

void WidgetPidAutotune::setPIDData(const PID& pid, int accuracy) {
    this->lAccuracyIndicate->setText(QString::number(accuracy / 100.0, 'f', 2));

    this->lPValue->setText(QString::number(pid.p, 'f', 2));
    this->lIValue->setText(QString::number(pid.i, 'f', 2));
    this->lDValue->setText(QString::number(pid.d, 'f', 2));

    this->update();
}

void WidgetPidAutotune::accept() {
    PID data;
    data.p = this->lPValue->text().toFloat();
    data.i = this->lIValue->text().toFloat();
    data.d = this->lDValue->text().toFloat();

    emit sendPID(data);
}
