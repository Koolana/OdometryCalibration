#ifndef MODELROBOTCONTROLLER_H
#define MODELROBOTCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QtSerialPort/QSerialPort>

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

signals:

};

#endif // MODELROBOTCONTROLLER_H
