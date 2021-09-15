#ifndef MODELROBOTCONTROLLER_H
#define MODELROBOTCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QtMath>
#include <QMutex>

#include "odomdatatype.h"

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
    int cmdTimeout = 500;
    bool clearFlag = false;

    RobotParams* rp;
    TestData* td;
    OdomDataType* odomPoint;

    void controlRobot();
    void moveInTest();
    void sendMoveCmd(float speedLinear, float speedRotate);
    void resetParams();

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
    void sendPID(const PID& data);

signals:
    void connected(bool);

    void sendOdomPoint(const OdomDataType& data);
    void sendFinalPoint(const OdomDataType& data);

};

#endif // MODELROBOTCONTROLLER_H
