#include "../include/widgetrobotsettings.h"

#include <QDebug>

WidgetRobotSettings::WidgetRobotSettings()
{
    this->setTitle("Robot settings");

    this->globalLayout = new QGridLayout();

    this->btnRotate = new QPushButton();
    this->btnRotate->setIcon(this->cwCircleArrow);
    this->btnRotate->setFixedSize(90, 90);
    this->btnRotate->setIconSize(QSize(80, 80));
    connect(this->btnRotate, SIGNAL(clicked()), this, SLOT(swapRotateDirection()));
    this->globalLayout->addWidget(this->btnRotate, 1, 0, 4, 4, Qt::AlignCenter | Qt::AlignBottom);

    auto lWidth = new QLabel("Width:");
    globalLayout->addWidget(lWidth, 5, 0, 1, 1);

    this->leWidth = new QLineEdit(QString::number(0.275));
    globalLayout->addWidget(this->leWidth, 6, 0, 1, 4);

    auto lLinSpeed = new QLabel("Linear speed:");
    globalLayout->addWidget(lLinSpeed, 7, 0, 1, 1);

    this->leLinSpeed = new QLineEdit(QString::number(0.1));
    globalLayout->addWidget(this->leLinSpeed, 8, 0, 1, 4);

    auto lAngSpeed = new QLabel("Angular speed:");
    globalLayout->addWidget(lAngSpeed, 9, 0, 1, 1);

    this->leAngSpeed = new QLineEdit(QString::number(0.3));
    globalLayout->addWidget(this->leAngSpeed, 10, 0, 1, 4);

    this->btnSave = new QPushButton("Save");
    connect(this->btnSave, SIGNAL(clicked()), this, SLOT(saveParams()));
    globalLayout->addWidget(this->btnSave, 11, 0, 1, 4);

    this->setLayout(this->globalLayout);
}

void WidgetRobotSettings::saveParams() {
    RobotParams rp;
    rp.width = this->leWidth->text().toFloat();
    rp.linearSpeed = this->leLinSpeed->text().toFloat();
    rp.angularSpeed = this->leAngSpeed->text().toFloat();

    emit savedParams(rp);
}

void WidgetRobotSettings::swapRotateDirection() {
    this->isCW = !this->isCW;

    if (this->isCW) {
        this->btnRotate->setIcon(this->cwCircleArrow);
    } else {
        this->btnRotate->setIcon(this->ccwCircleArrow);
    }

    emit changedRotateDir(this->isCW);
}
