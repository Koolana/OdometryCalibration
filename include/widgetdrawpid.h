#ifndef WIDGETDRAWPID_H
#define WIDGETDRAWPID_H

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QTimer>

#include "odomdatatype.h"

class WidgetDrawPID : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetDrawPID(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    QVector<QPointF> trajPoints;

    int numVerticalLine = 10;
    int numHorizontalLine = 10;

    float scaleDivY = 0.1;
    float scaleDivX;

    int msecs = 10000;
    int period = 100;

    bool clearFlag = false;

    void drawScale(QPainter& qp);
    void drawTrajectory(QPainter& qp);

public slots:
    void addTrajPoint(const OdomDataType& data);
    void clear();

};

#endif // WIDGETDRAWPID_H
