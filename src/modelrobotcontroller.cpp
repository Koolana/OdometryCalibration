#include "../include/modelrobotcontroller.h"

#include <QDebug>

ModelRobotController::ModelRobotController(QObject *parent) : QObject(parent)
{
//    this->fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

//    QThread::sleep(1);

//    char* sendStr = "v1 0\n";
//    int size = strlen(sendStr);
//    qDebug() << write(fd, sendStr, size);
//    QThread::sleep(1);

//    for (int i = 0; i < 10; i++) {
//        char c;
//        QString str("");

//        do {
//            read(fd, &c, 1);
//            str += c;
//        } while(c != '\n');

//        qDebug() << str;
//        QThread::sleep(1);
//    }

//    char* sendNewStr = "n\n";
//    size = strlen(sendNewStr);
//    qDebug() << write(fd, sendNewStr, size);

    FILE* file = fopen("/dev/ttyACM0", "r+");
    QThread::sleep(1);

    fprintf(file, "v1 0\n");
    fflush(file);

    for (int i = 0; i < 5; i++) {
        char c[36];

        fread(c, sizeof(char), 36, file);

        qDebug() << QString(c);
        QThread::sleep(1);
    }

    QThread::sleep(1);
    fprintf(file, "n/n");
    fflush(file);

    fclose(file);
}

ModelRobotController::~ModelRobotController() {
    close(fd);
}
