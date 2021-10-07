#include "../include/modelrobotcontroller.h"

#include <QDebug>

ModelRobotController::ModelRobotController(QObject *parent) : QObject(parent)
{
}

void ModelRobotController::init() {
    this->state = 0;

    this->inputMsg = new QByteArray();
    this->mutex = new QMutex();

    this->rp = new RobotParams;
    this->td = new TestData;
    this->odomPoint = new OdomDataType;

    this->qsp = new QSerialPort();
    connect(this->qsp, SIGNAL(readyRead()), this, SLOT(receiveFromSerial()));

    this->robotTimer = new QTimer();
    connect(this->robotTimer, SIGNAL(timeout()), this, SLOT(pollRobot()));
}

void ModelRobotController::openDevice(const QString& devPath) {
    if (this->qsp->isOpen()) {
        this->robotTimer->stop();
        this->qsp->close();
    }

    this->qsp->setPortName(devPath);

    if (this->qsp->open(QIODevice::ReadWrite)) {
        this->qsp->setBaudRate(QSerialPort::Baud57600);
        this->qsp->setDataBits(QSerialPort::Data8);
        this->qsp->setParity(QSerialPort::NoParity);
        this->qsp->setStopBits(QSerialPort::OneStop);
        this->qsp->setFlowControl(QSerialPort::NoFlowControl);

        emit connected(true);

        QThread::sleep(1);

        this->qsp->clear();

        this->msgReceived = true;
        this->clearFlag = false;
        this->resetParams();

        this->robotTimer->start(100);
    } else {
        emit connected(false);
    }
}

void ModelRobotController::pollRobot() {
    if (!this->qsp->isOpen()) {
        this->robotTimer->stop();
        return;
    }

    if (this->msgReceived) {
        if (this->clearFlag) {
            this->clearFlag = false;
        }

        this->inputMsg->clear();
        this->qsp->clear();

        this->mutex->lock();

        this->qsp->write("ot");
        this->qsp->flush();

        this->mutex->unlock();

        this->msgReceived = false;
    }
}

void ModelRobotController::receiveFromSerial() {
    this->inputMsg->append(this->qsp->readAll());

    if (this->inputMsg->at(this->inputMsg->count() - 1) == '\n') {
        this->msgReceived = true;
        this->controlRobot();
    }
}

void ModelRobotController::controlRobot() {
    QStringList parsedMsg = QString(this->inputMsg->data()).split(';');

//    qDebug() << parsedMsg;

    if (parsedMsg.count() < 6) {
        return;
    }

    this->odomPoint->x = parsedMsg.at(3).toFloat();
    this->odomPoint->y = parsedMsg.at(4).toFloat();
    this->odomPoint->th = parsedMsg.at(2).toFloat();

    this->odomPoint->vx = parsedMsg.at(0).toFloat();
    this->odomPoint->vy = 0;
    this->odomPoint->vth = parsedMsg.at(1).toFloat();

    emit sendOdomPoint(*this->odomPoint);

    this->currL += qSqrt(qPow(this->prevX - this->odomPoint->x, 2) + qPow(this->prevY - this->odomPoint->y, 2));
    this->currTh += qFabs(this->prevTh - this->odomPoint->th);

    this->moveInTest();

    this->prevX = this->odomPoint->x;
    this->prevY = this->odomPoint->y;
    this->prevTh = this->odomPoint->th;

    if (parsedMsg.count() == 10) {
        PID pid;
        pid.p = parsedMsg.at(6).toFloat();
        pid.i = parsedMsg.at(7).toFloat();
        pid.d = parsedMsg.at(8).toFloat();

//        qDebug() << parsedMsg.at(5).toInt();

        emit sendPIDwihAccuracy(pid, parsedMsg.at(5).toInt());
    }
}

void ModelRobotController::moveInTest() {
//    qDebug() << this->state << this->currL << this->currTh;

    switch (td->typeTest) {
    case Tests::LINE:
        if (this->state == 1) {
            this->state = 2;
            this->sendMoveCmd(this->rp->linearSpeed, 0);
        }

        if (this->state == 2 && this->currL + qFabs(this->odomPoint->vx) / (1000 / this->robotTimer->interval()) >= this->td->size) {
            this->finish = true;
            this->state = 0;
            this->currL = 0;
            this->sendStopCmd();
        }

        if (qFabs(this->odomPoint->vx) < 0.01 && this->finish) {
            this->odomPoint->numIter++;
            emit this->sendFinalPoint(*this->odomPoint);
            this->finish = false;
        }

        break;

    case Tests::SQUARE:
        switch (this->state) {
        case 0:
            break;

        case 1:
            this->state++;
            this->sendMoveCmd(this->rp->linearSpeed, 0);
            break;

        case 2:
        case 4:
        case 6:
        case 8:
            if (this->currL + qFabs(this->odomPoint->vx) / (1000 / this->robotTimer->interval()) >= this->td->size) {
                this->state++;
                this->sendStopCmd();
                this->currTh = 0;
                this->sendMoveCmd(0, (this->odomPoint->isClockwise ? -1 : 1) * this->rp->angularSpeed);
            }
            break;

        case 3:
        case 5:
        case 7:
        case 9:
            if (this->currTh + qFabs(this->odomPoint->vth) / (1000 / this->robotTimer->interval()) >= M_PI / 2) {
                this->state++;
                this->sendStopCmd();
                this->currL = 0;

                if (this->state == 10) {
                    this->odomPoint->numIter++;

                    if (!(this->odomPoint->numIter % this->td->numIter == 0)) {
                        this->state = 1;
                    }
                } else {
                    this->sendMoveCmd(this->rp->linearSpeed, 0);
                }
            }
            break;
        case 10:
            if (qFabs(this->odomPoint->vx) < 0.01) {
                this->odomPoint->numIter = this->odomPoint->numIter / this->td->numIter;

                emit this->sendFinalPoint(*this->odomPoint);

                this->odomPoint->numIter = this->odomPoint->numIter * this->td->numIter;

                this->state = 0;
            }
        }

        break;

    case Tests::CIRCLE:
        switch (this->state) {
        case 0:
            break;
        case 1:
            this->state++;
            this->currTh = 0;
            this->sendMoveCmd(this->rp->linearSpeed, (this->odomPoint->isClockwise ? -1 : 1) * this->rp->linearSpeed / this->td->size);

            break;

        case 2:
            if (this->currTh + qFabs(this->odomPoint->vth) / (1000 / this->robotTimer->interval()) >= 2 * M_PI) {
                this->odomPoint->numIter++;

                if (!(this->odomPoint->numIter % this->td->numIter == 0)) {
                    this->state = 1;
                } else {
                    this->state = 3;
                    this->sendStopCmd();
                }
            }
            break;

        case 3:
            if (qFabs(this->odomPoint->vx) < 0.01 and qFabs(this->odomPoint->vth) < 0.01) {
                this->odomPoint->numIter = this->odomPoint->numIter / this->td->numIter;

                emit this->sendFinalPoint(*this->odomPoint);

                this->odomPoint->numIter = this->odomPoint->numIter * this->td->numIter;

                this->state = 0;
            }
            break;
        }

        case Tests::ERROR:
            break;
        }
}

void ModelRobotController::sendMoveCmd(float speedLinear, float speedRotate) {
    if (!this->qsp->isOpen()) {
        return;
    }

    this->prevSpeedLinear = speedLinear;
    this->prevSpeedRotate = speedRotate;

    QString msg = "v" + QString::number(speedLinear, 'f', 2) + " " + QString::number(speedRotate, 'f', 2) + "\n";

    this->mutex->lock();

    this->qsp->write(msg.toUtf8());
    this->qsp->flush();
    QThread::msleep(this->cmdTimeout);

    this->mutex->unlock();

//    qDebug() << "Move msg:" << msg;
}

void ModelRobotController::sendStopCmd() {
    if (!this->qsp->isOpen()) {
        return;
    }

    this->mutex->lock();

    this->qsp->write("p");
    this->qsp->flush();
    QThread::msleep(this->cmdTimeout);

    this->mutex->unlock();

//    qDebug() << "Move msg:" << "p";
}

void ModelRobotController::sendStartCmd() {
    if (!this->qsp->isOpen()) {
        return;
    }

    if (this->state == 0) {
        this->state = 1;
    } else {
        this->sendMoveCmd(this->prevSpeedLinear, this->prevSpeedRotate);
    }
}

void ModelRobotController::sendResetCmd() {
    if (!this->qsp->isOpen()) {
        return;
    }

    this->sendStopCmd();

    this->resetParams();

    this->mutex->lock();

    this->qsp->write("n");
    this->qsp->flush();
    this->clearFlag = true;
    QThread::msleep(this->cmdTimeout);

    this->mutex->unlock();

    this->resetParams();

//    qDebug() << "Move msg:" << "n";
}

void ModelRobotController::sendPIDSetCmd(const PID& data) {
    QString msg = "k" + QString::number(data.p, 'f', 5) + " " +
                        QString::number(data.i, 'f', 5) + " " +
                        QString::number(data.d, 'f', 5) + "\n";

    this->mutex->lock();

    this->qsp->write(msg.toUtf8());
    this->qsp->flush();
    QThread::msleep(this->cmdTimeout);

    this->mutex->unlock();
}

void ModelRobotController::sendPIDTuneCmd() {
    QString msg = "a";

    this->mutex->lock();

    this->qsp->write(msg.toUtf8());
    this->qsp->flush();
    QThread::msleep(this->cmdTimeout);

    this->mutex->unlock();
}

void ModelRobotController::resetParams() {
    this->state = 0;
    this->currL = 0;
    this->currTh = 0;

    this->prevX = 0;
    this->prevY = 0;
    this->prevTh = 0;

    this->odomPoint->numIter = 0;
}

void ModelRobotController::changeRotateDir(bool isCW) {
    this->odomPoint->isClockwise = isCW;

//    qDebug() << "Rotate changed:" << this->odomPoint->isClockwise;
}

void ModelRobotController::changeRobotParams(const RobotParams& data) {
    rp->angularSpeed = data.angularSpeed;
    rp->linearSpeed = data.linearSpeed;
    rp->width = data.width ;

    if (td->typeTest == Tests::CIRCLE) {
        this->prevSpeedRotate = this->prevSpeedRotate ? this->prevSpeedRotate * qFabs(this->rp->linearSpeed / this->prevSpeedLinear) : 0;
    } else{
        this->prevSpeedRotate = this->prevSpeedRotate ? this->prevSpeedRotate * qFabs(this->rp->angularSpeed / this->prevSpeedRotate) : 0;
    }

    this->prevSpeedLinear = this->prevSpeedLinear ? this->prevSpeedLinear * qFabs(this->rp->linearSpeed / this->prevSpeedLinear) : 0;

    this->sendStartCmd();

//    qDebug() << "Robot params changed" << rp->linearSpeed << rp->angularSpeed;
}

void ModelRobotController::changeTestData(const TestData& data) {
    td->numIter = data.numIter;
    td->size = data.size;
    td->typeTest = data.typeTest;

//    qDebug() << "Test params changed" << td->size << td->typeTest;
}

ModelRobotController::~ModelRobotController() {
    if (this->qsp->isOpen()) {
        this->qsp->close();
    }
}
