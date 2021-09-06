#include "../include/modelrobotcontroller.h"

#include <QDebug>

ModelRobotController::ModelRobotController(QObject *parent) : QObject(parent)
{
    this->qsp = new QSerialPort("/dev/ttyACM1");

    this->qsp->open(QIODevice::ReadWrite);

    this->qsp->setBaudRate(QSerialPort::Baud57600);
    this->qsp->setDataBits(QSerialPort::Data8);
    this->qsp->setParity(QSerialPort::NoParity);
    this->qsp->setStopBits(QSerialPort::OneStop);
    this->qsp->setFlowControl(QSerialPort::NoFlowControl);
//    this->qsp->setDataTerminalReady(true);
//    this->qsp->setRequestToSend(true);

    QThread::sleep(1);

    while (true) {
        this->qsp->clear();

        QByteArray msg("o");
        this->qsp->write(msg);
        this->qsp->flush();

        QThread::msleep(500);

        char c = ' ';
        QString str("");

        do {
            if(this->qsp->waitForReadyRead(10000)) {
                this->qsp->read(&c, 1);
                str += c;
            }
        } while(c != '\n');

        if (str.split(';').count() == 6) {
            qDebug() << str;
        }
    }

    this->qsp->write("v1 0\n");
    this->qsp->waitForBytesWritten(10000);

    qDebug() << this->qsp->isWritable();
}

ModelRobotController::~ModelRobotController() {
    this->qsp->close();
}
