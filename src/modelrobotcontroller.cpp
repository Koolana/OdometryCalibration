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

    this->qsp = new QSerialPort("/dev/ttyACM0");
    connect(this->qsp, SIGNAL(readyRead()), this, SLOT(receiveFromSerial()));

    this->qsp->open(QIODevice::ReadWrite);

    this->qsp->setBaudRate(QSerialPort::Baud57600);
    this->qsp->setDataBits(QSerialPort::Data8);
    this->qsp->setParity(QSerialPort::NoParity);
    this->qsp->setStopBits(QSerialPort::OneStop);
    this->qsp->setFlowControl(QSerialPort::NoFlowControl);

    QThread::sleep(1);

    this->robotTimer = new QTimer();
    connect(this->robotTimer, SIGNAL(timeout()), this, SLOT(pollRobot()));
    this->robotTimer->start(100);

    this->qsp->clear();
}

void ModelRobotController::pollRobot() {
    if (msgReceived) {
        this->controlRobot();

        this->inputMsg->clear();
        this->qsp->clear();

        this->mutex->lock();

        this->qsp->write("o");
        this->qsp->flush();

        this->mutex->unlock();

        msgReceived = false;
    }
}

void ModelRobotController::receiveFromSerial() {
    this->inputMsg->append(this->qsp->readAll());

    if (this->inputMsg->at(this->inputMsg->count() - 1) == '\n') {
        msgReceived = true;
    }
}

void ModelRobotController::controlRobot() {
    QStringList parsedMsg = QString(this->inputMsg->data()).split(';');

    if (!(parsedMsg.count() == 6)) {
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

    this->moveInTest();

    this->prevX = odomPoint->x;
    this->prevY = odomPoint->y;
}

void ModelRobotController::moveInTest() {
    switch (td->typeTest) {
    case Tests::LINE:
        if (this->state == 1) {
            this->state = 2;
            this->sendMoveCmd(this->rp->linearSpeed, 0);
        }

        if (this->state == 2 && this->currL > this->td->size) {
            this->finish = true;
            this->state = 0;
            this->currL = 0;
            this->sendStopCmd();
        }

        if (qFabs(this->odomPoint->vx) < 0.01 and this->finish) {
            emit this->sendFinalPoint(*this->odomPoint);
            this->odomPoint->numIter += 1;
            this->finish = false;
        }

        break;
    case Tests::SQUARE:
        break;
    case Tests::CIRCLE:
        break;
    case Tests::ERROR:
        break;
    }
//    if self.testType == 1:
//        if self.state == 1:
//            self.state = 2
//            self.move_callback(self.maxSpeed, 0)

//        if self.state == 2 and self.currL + vx / 10 > self.a:
//            self.state = 3
//            self.stopMoving()
//            self.move_callback(0, (-1 if self.isClockwise else 1) * self.maxRotate)

//        if self.state == 3 and abs(th + vth / 10) > math.pi / 2 + 2 * math.pi * self.numIter:
//            self.state = 4
//            self.stopMoving()
//            self.currL = 0
//            self.move_callback(self.maxSpeed, 0)

//        if self.state == 4 and self.currL + vx / 10 > self.a:
//            self.state = 5
//            self.stopMoving()
//            self.move_callback(0, (-1 if self.isClockwise else 1) * self.maxRotate)

//        if self.state == 5 and abs(th + vth / 10) > math.pi + 2 * math.pi * self.numIter:
//            self.state = 6
//            self.stopMoving()
//            self.currL = 0
//            self.move_callback(self.maxSpeed, 0)

//        if self.state == 6 and self.currL + vx / 10 > self.a:
//            self.state = 7
//            self.stopMoving()
//            self.move_callback(0, (-1 if self.isClockwise else 1) * self.maxRotate)

//        if self.state == 7 and abs(th + vth / 10) > 3 * math.pi / 2 + 2 * math.pi * self.numIter:
//            self.state = 8
//            self.stopMoving()
//            self.currL = 0
//            self.move_callback(self.maxSpeed, 0)

//        if self.state == 8 and self.currL + vx / 10 > self.a:
//            self.state = 9
//            self.stopMoving()
//            self.move_callback(0, (-1 if self.isClockwise else 1) * self.maxRotate)

//        if self.state == 9 and abs(th + vth / 10) > 2 * math.pi + 2 * math.pi * self.numIter:
//            self.currL = 0
//            self.numIter += 1
//            self.c.changeIter.emit(self.numIter)

//            if  self.numIter == self.maxIter:
//                self.state = 0
//                self.finish = True
//            else:
//                self.state = 1

//            self.stopMoving()

//        if abs(vx) < 0.01 and self.finish:
//            self.c.finalPosition.emit([x, y, th])
//            self.finish = False

//    if self.testType == 2:
//        if self.state == 1:
//            self.state = 2

//            if self.isClockwise:
//                self.move_callback(self.maxSpeed, -self.maxSpeed / self.a)
//            else:
//                self.move_callback(self.maxSpeed, self.maxSpeed / self.a)

//        if self.state == 2 and abs(th) > 2 * math.pi * (1 + self.numIter):
//            self.numIter += 1
//            self.c.changeIter.emit(self.numIter)

//            if  self.numIter == self.maxIter:
//                self.finish = True
//                self.state = 0
//                self.stopMoving()

//        if abs(vx) < 0.01 and abs(vth) < 0.01 and self.finish:
//            self.c.finalPosition.emit([x, y, th])
//            self.finish = False
}

void ModelRobotController::sendMoveCmd(float speedLinear, float speedRotate) {
    this->prevSpeedLinear = speedLinear;
    this->prevSpeedRotate = speedRotate;

    QString msg = "v" + QString::number(speedLinear, 'f', 2) + " " + QString::number(speedRotate, 'f', 2) + "\n";

    this->mutex->lock();

    this->qsp->write(msg.toUtf8());
    this->qsp->flush();
    QThread::msleep(200);

    this->mutex->unlock();

    qDebug() << "Move msg:" << msg;
}

void ModelRobotController::sendStopCmd() {
    this->mutex->lock();

    this->qsp->write("p");
    this->qsp->flush();
    QThread::msleep(200);

    this->mutex->unlock();

    qDebug() << "Move msg:" << "p";
}

void ModelRobotController::sendStartCmd() {
    qDebug() << this->state;
    if (this->state == 0) {
        this->state = 1;
    } else {
        this->sendMoveCmd(this->prevSpeedLinear, this->prevSpeedRotate);
    }
}

void ModelRobotController::sendResetCmd() {
    this->mutex->lock();

    this->qsp->write("n");
    this->qsp->flush();
    QThread::msleep(200);

    this->mutex->unlock();

    qDebug() << "Move msg:" << "n";
}

void ModelRobotController::changeRotateDir(bool isCW) {
    this->isCW = isCW;
}

void ModelRobotController::changeRobotParams(const RobotParams& data) {
    rp->angularSpeed = data.angularSpeed;
    rp->linearSpeed = data.linearSpeed;
    rp->width = data.width ;

    qDebug() << "Robot params changed" << rp->linearSpeed << rp->angularSpeed;
}

void ModelRobotController::changeTestData(const TestData& data) {
    td->numIter = data.numIter;
    td->size = data.size;
    td->typeTest = data.typeTest;

    qDebug() << "Test params changed" << td->size << td->typeTest;
}

ModelRobotController::~ModelRobotController() {
//    QByteArray msg("p");
//    this->qsp->write(msg);
//    this->qsp->flush();

    this->qsp->close();
}
