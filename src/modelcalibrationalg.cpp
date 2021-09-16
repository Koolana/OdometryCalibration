#include "../include/modelcalibrationalg.h"

#include <QDebug>

ModelCalibrationAlg::ModelCalibrationAlg(QObject *parent) : QObject(parent)
{

}

void ModelCalibrationAlg::calc(QVector<FullData>& data) {
    CalibrationResults res;
    res.typeTest = this->currentTest.typeTest;

    switch (this->currentTest.typeTest) {
    case Tests::LINE: {
        qreal sumDeltaX = 0;
        qreal sumDeltaY = 0;

        for (int i = 0; i < data.count(); i++) {
            FullData currData = data.at(i);

            sumDeltaX += currData.exper.x - currData.odom.x;
            sumDeltaY += currData.exper.y - currData.odom.y;
        }

        if (sumDeltaX == 0) {
            res.typeTest = Tests::ERROR;
            emit sendResult(res);
            return;
        }

        qreal half_betta = qAtan(sumDeltaY / sumDeltaX);

        qreal r = 1000 / qSin(half_betta);

        qreal Ed = (r + 12.3 / 2) / (r - 12.3 / 2);

        if ((Ed + 1) == 0) {
            res.CL = 0;
        } else {
            res.CL = (2 / (Ed + 1));
        }

        if (((1/Ed)+1) == 0) {
            res.CR = 0;
        } else {
            res.CR = 2 / ((1 / Ed) + 1);
        }

        break;
    }
    case Tests::SQUARE: {
        if (robot_width == 0) {
            res.typeTest = Tests::ERROR;
            emit sendResult(res);
            return;
        }

        qreal sumDeltaX_cw = 0;
        qreal sumDeltaY_cw = 0;

        qreal sumDeltaX_ccw = 0;
        qreal sumDeltaY_ccw = 0;

        int cwCount = 0;
        int ccwCount = 0;

        for (int i = 0; i < data.count(); i++) {
            FullData currData = data.at(i);

//            qDebug() << currData.odom.isClockwise << currData.exper.x << currData.odom.x;

            if (currData.odom.isClockwise) {
                sumDeltaX_cw += currData.exper.x - currData.odom.x;
                sumDeltaY_cw += currData.exper.y - currData.odom.y;
                cwCount++;
            } else {
                sumDeltaX_ccw += currData.exper.x - currData.odom.x;
                sumDeltaY_ccw += currData.exper.y - currData.odom.y;
                ccwCount++;
            }
        }

        sumDeltaX_cw /= cwCount;
        sumDeltaY_cw /= cwCount;

        sumDeltaX_ccw /= ccwCount;
        sumDeltaY_ccw /= ccwCount;

        qreal betta = (sumDeltaX_cw - sumDeltaX_ccw) / ((-4) * this->currentTest.size);

        qreal R = (betta == 0 ? 0 : (this->currentTest.size / 2) / qSin(betta / 2));

        qreal Ed = (R + this->robot_width / 2) / (R - this->robot_width / 2);

        qreal alpha = (sumDeltaX_cw + sumDeltaX_ccw) / ((-4) * this->currentTest.size);

        qreal Eb = (M_PI / 2) / ((M_PI / 2) - alpha);

        res.b = Eb * this->robot_width;

        res.CL = ((Ed + 1) == 0 ? 0 : (2 / (Ed + 1)));
        res.CR = (((1 / Ed) + 1) == 0 ? 0 : (2 / ((1 / Ed) + 1)));

        break;
    }
    case Tests::CIRCLE:
        break;
    case Tests::ERROR:
        break;
    }

    emit sendResult(res);
}

void ModelCalibrationAlg::changeTest(const TestData& data) {
    this->currentTest = data;
}

void ModelCalibrationAlg::changeRobotWidth(const RobotParams& data) {
    this->robot_width = data.width;
}
