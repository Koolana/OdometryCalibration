#ifndef WIDGETDRAWPID_H
#define WIDGETDRAWPID_H

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QTimer>
#include <QGraphicsWidget>
#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>
#include <QGridLayout>

#include "odomdatatype.h"

class WidgetDrawPID : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetDrawPID(QWidget *parent = nullptr);

private:
    QVector<QPointF> trajPoints;

    int numVerticalLine = 11;
    int numHorizontalLine = 11;

    QtCharts::QLineSeries *series;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;

    int msecs = 10000;
    int period = 100;

    bool clearFlag = false;

public slots:
    void addTrajPoint(const OdomDataType& data);
    void clear();

};

#endif // WIDGETDRAWPID_H
