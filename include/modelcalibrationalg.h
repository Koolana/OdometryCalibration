#ifndef MODELCALIBRATIONALG_H
#define MODELCALIBRATIONALG_H

#include <QObject>
#include <QtMath>

#include "odomdatatype.h"

class ModelCalibrationAlg : public QObject
{
    Q_OBJECT
public:
    explicit ModelCalibrationAlg(QObject *parent = nullptr);

private:
    TestData currentTest;
    qreal robot_width = 0;

signals:
    void sendResult(CalibrationResults&);

public slots:
    void calc(QVector<FullData>& data);
    void changeTest(const TestData&);
    void changeRobotWidth(const RobotParams&);
};

#endif // MODELCALIBRATIONALG_H
