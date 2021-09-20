#include "include/widgetdrawpid.h"

#include <QDebug>

WidgetDrawPID::WidgetDrawPID(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());

    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    this->series = new QtCharts::QSplineSeries();

    this->chartView = new QtCharts::QChartView();

    this->chart = new QtCharts::QChart();
    this->chart->addSeries(series);
    this->chart->legend()->hide();

    this->chart->setToolTip(QString("График WW\n"
                              "Количество отсчётов VV"));

    QtCharts::QValueAxis *axisX = new QtCharts::QValueAxis();
    axisX->setTitleText("time, ms");
    axisX->setLabelFormat("%i");
    axisX->setRange(-this->msecs, 0);
    axisX->setTickCount(this->numVerticalLine);
    this->chart->addAxis(axisX, Qt::AlignBottom);
    this->series->attachAxis(axisX);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("speed, m/sec");
    axisY->setRange(-0.5, 0.5);
    axisY->setLabelFormat("%.2f");
    axisY->setTickCount(this->numHorizontalLine);
    this->chart->addAxis(axisY, Qt::AlignLeft);
    this->series->attachAxis(axisY);

    this->chartView->setChart(this->chart);

    auto layout = new QGridLayout();
    layout->addWidget(this->chartView);
    this->setLayout(layout);
}

void WidgetDrawPID::addTrajPoint(const OdomDataType& data) {
    if (this->clearFlag) {
        this->clearFlag = false;
        return;
    }

    QPointF newPoint;
    newPoint.setX(0);
    newPoint.setY(data.vx);

    for (int i = 0; i < this->series->count(); i++) {
        if (this->series->at(i).x() < -this->msecs) {
            this->series->remove(i);
        } else {
            this->series->replace(i, this->series->at(i).x() - this->period,
                                     this->series->at(i).y());

        }
    }

    *this->series << newPoint;

    this->update();
}

void WidgetDrawPID::clear() {
    this->series->clear();

    this->clearFlag = true;

    this->update();
}
