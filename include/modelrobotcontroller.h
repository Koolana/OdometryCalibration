#ifndef MODELROBOTCONTROLLER_H
#define MODELROBOTCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QtMath>
#include <QMutex>
#include <QTime>

#include "odomdatatype.h"

class ModelRobotController : public QObject
{
    Q_OBJECT
public:
    explicit ModelRobotController(QObject *parent = nullptr);
    ~ModelRobotController();

private:
    enum CmdTypes
    {
        LineMove,
        RotateMove,
    };

    QSerialPort *qsp;
    QByteArray *inputMsg;
    QMutex *mutex;

    bool msgReceived = true;

    QTimer *robotTimer;
    QTimer *checkConnectTimer;

    float prevX = 0;
    float prevY = 0;
    float prevTh = 0;

    float prevSpeedLinear = 0;
    float prevSpeedRotate = 0;

    float currL = 0;
    float currTh = 0;

    int state = 0;
    bool finish = false;
    int cmdTimeout = 500;
    bool clearFlag = false;
    bool isPaused = false;
    bool isInitState = true;
    bool isPIDTuneState = false;

    bool inProgress = false;
    bool progressIsChanged = false;
    bool progressReaded = true;

    RobotParams* rp;
    TestData* td;
    OdomDataType* odomPoint;

    Tests prevTest = Tests::ERROR;

    void controlRobot();
    void moveInTest();
    void sendTestType(CmdTypes type, double val);
    void sendMoveCmd(float speedLinear, float speedRotate);
    void resetParams();

private slots:
    void receiveFromSerial();
    void pollRobot();
    void timeoutRobot();

public slots:
    void init();
    void openDevice(const QString& devPath);

    void changeRotateDir(bool);
    void changeRobotParams(const RobotParams&);
    void changeTestData(const TestData&);
    void changeTestMode(bool isPID);

    void sendStartCmd();
    void sendResetCmd();
    void sendStopCmd();
    void sendPIDSetCmd(const PID& data);
    void sendPIDTuneCmd();

signals:
    void connected(bool);

    void sendOdomPoint(const OdomDataType& data);
    void sendFinalPoint(const OdomDataType& data);
    void sendPIDwihAccuracy(const PID& pid, int accuracy);

};

#endif // MODELROBOTCONTROLLER_H
