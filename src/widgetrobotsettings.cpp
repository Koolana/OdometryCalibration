#include "../include/widgetrobotsettings.h"

#include <QDebug>

WidgetRobotSettings::WidgetRobotSettings()
{
    this->setTitle("Robot settings");

    this->globalLayout = new QGridLayout();

    this->lePathToDevice = new QLineEdit("ttyACM0");
    globalLayout->addWidget(this->lePathToDevice, 0, 0, 1, 3);

    this->btnOpenDevice = new QPushButton("Connect");
    connect(this->btnOpenDevice, SIGNAL(clicked()), this, SLOT(openDevice()));
    globalLayout->addWidget(this->btnOpenDevice, 0, 3, 1, 1);

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

void WidgetRobotSettings::init() {
    this->saveParams();
    emit changedRotateDir(this->isCW);
    this->openDevice();
}

void WidgetRobotSettings::displayConnectInfo(bool isConnected) {
    QPalette pal = this->btnOpenDevice->palette();
    pal.setColor(QPalette::Button, isConnected ? QColor(Qt::green) : QColor(Qt::red));
    this->btnOpenDevice->setAutoFillBackground(true);
    this->btnOpenDevice->setPalette(pal);
    this->btnOpenDevice->update();
}

void WidgetRobotSettings::openDevice() {
    emit sendDevicePath(this->lePathToDevice->text());
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
