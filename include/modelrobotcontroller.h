#ifndef MODELROBOTCONTROLLER_H
#define MODELROBOTCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QtMath>
#include <QMutex>

#include "odomdatatype.h"

//#include <iostream>
//#include <unistd.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <fcntl.h>
//#include <QString>
//#include <QThread>
//#include <iostream>
//#include <stdio.h>

class ModelRobotController : public QObject
{
    Q_OBJECT
public:
    explicit ModelRobotController(QObject *parent = nullptr);
    ~ModelRobotController();

private:
    QSerialPort *qsp;
    QByteArray *inputMsg;
    QMutex *mutex;

    bool msgReceived = true;

    QTimer *robotTimer;

    float prevX = 0;
    float prevY = 0;
    float prevTh = 0;

    float prevSpeedLinear;
    float prevSpeedRotate;

    float currL = 0;
    float currTh = 0;

    int state = 0;
    bool finish = false;

    RobotParams* rp;
    TestData* td;
    OdomDataType* odomPoint;

    void controlRobot();
    void moveInTest();
    void sendMoveCmd(float speedLinear, float speedRotate);

private slots:
    void receiveFromSerial();
    void pollRobot();

public slots:
    void init();
    void openDevice(const QString& devPath);

    void changeRotateDir(bool);
    void changeRobotParams(const RobotParams&);
    void changeTestData(const TestData&);

    void sendStartCmd();
    void sendResetCmd();
    void sendStopCmd();

signals:
    void connected(bool);

    void sendOdomPoint(const OdomDataType& data);
    void sendFinalPoint(const OdomDataType& data);

};

#endif // MODELROBOTCONTROLLER_H
