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

    int numVerticalLine = 10;
    int numHorizontalLine = 10;
    float scaleDiv = 0.5;

    void drawScale(QPainter& qp);

};

#endif // WIDGETDRAWPID_H
