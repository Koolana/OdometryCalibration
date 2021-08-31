#ifndef WIDGETDRAW_H
#define WIDGETDRAW_H

#include <QWidget>
#include <QPainter>
#include <QtMath>

#include "odomdatatype.h"

class WidgetDraw : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetDraw(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    int numVerticalLine = 10;
    int numHorizontalLine = 10;
    float scaleDiv = 0.5;

    float robotLenX = 0.25;
    float robotLenY = 0.25;

    float drawRobotWidth = 0;
    float drawRobotHeight = 0;

    OdomDataType robotPos;
    float robotAng = 0;

    TestData currTest;
    bool isCW = true;

    void drawScale(QPainter& qp);
    void drawRobot(QPainter& qp);
    void drawTarget(QPainter& qp);
    void drawTrajectory(QPainter& qp);
    void drawCurrentRobotInfo(QPainter& qp);

public slots:
    void changeTest(TestData&);
    void changeRotateDir(bool isCW);

};

#endif // WIDGETDRAW_H
