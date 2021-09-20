#include "include/widgetdrawpid.h"

#include <QDebug>

WidgetDrawPID::WidgetDrawPID(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());

    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    this->scaleDivX = msecs / numVerticalLine;
}

void WidgetDrawPID::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter qp(this);
    this->drawScale(qp);
    this->drawTrajectory(qp);
}

void WidgetDrawPID::resizeEvent(QResizeEvent* event) {
    Q_UNUSED(event);

    if (this->size().width() > this->size().height()) {
        this->numHorizontalLine = 10;

        this->numVerticalLine = int(10 * this->size().width() / this->size().height());
        this->numVerticalLine += this->numVerticalLine % 2 == 0 ? 0 : 1;
    } else {
        this->numHorizontalLine = int(10 * this->size().height() / this->size().width());
        this->numHorizontalLine += this->numHorizontalLine % 2 == 0 ? 0 : 1;

        this->numVerticalLine = 10;
    }

    this->update();
}

void WidgetDrawPID::drawScale(QPainter& qp) {
    qp.save();

    QPen pen;
    pen.setWidth(1);
    qp.setPen(pen);

    for (int vl = 1; vl < this->numVerticalLine; vl++) {
        qp.rotate(-90);
        qp.drawText(-this->size().height() + 10,
                     this->size().width() / this->numVerticalLine * vl - 2,
                     QString::number((this->numVerticalLine - vl) * this->scaleDivX) + " ms");
        qp.rotate(90);
        qp.drawLine(float(this->size().width()) / float(this->numVerticalLine) * vl,
                     0,
                     float(this->size().width()) / float(this->numVerticalLine) * vl,
                     this->size().height());
    }

    for (int hl = 1; hl < this->numHorizontalLine; hl++) {
        if (hl == int(this->numHorizontalLine / 2)) {
            pen.setWidth(2);
            qp.setPen(pen);
        } else {
            pen.setWidth(1);
            qp.setPen(pen);
        }

        qp.drawText(10,
                     this->size().height() / this->numHorizontalLine * hl - 2,
                     QString::number((this->numHorizontalLine / 2 - hl) * this->scaleDivY) + " m/sec");
        qp.drawLine(0,
                     float(this->size().height()) / float(this->numHorizontalLine) * hl,
                     this->size().width(),
                     float(this->size().height()) / float(this->numHorizontalLine) * hl);
    }

    qp.restore();
}

void WidgetDrawPID::drawTrajectory(QPainter& qp) {
    if (this->trajPoints.count() < 2) {
        return;
    }

    qp.save();

    QPen pen(QColor(0, 0, 255), 2, Qt::SolidLine);
    qp.setPen(pen);

    QPointF prevPoint = this->trajPoints.first();

    for (int i = 1; i < this->trajPoints.count(); i++) {
        const QPointF* currPoint = &this->trajPoints.at(i);

        qp.drawLine(prevPoint.x() / this->scaleDivX * this->size().width() / this->numVerticalLine + this->size().width(),
        -prevPoint.y() / this->scaleDivY * this->size().height() / this->numHorizontalLine + this->size().height() / 2,
        currPoint->x() / this->scaleDivX * this->size().width() / this->numVerticalLine + this->size().width(),
        -currPoint->y() / this->scaleDivY * this->size().height() / this->numHorizontalLine + this->size().height() / 2);

        prevPoint = *currPoint;
    }

    qp.restore();
}

void WidgetDrawPID::addTrajPoint(const OdomDataType& data) {
    if (this->clearFlag) {
        this->clearFlag = false;
        return;
    }

    QPointF newPoint;
    newPoint.setX(0);
    newPoint.setY(data.vx);

    if (this->trajPoints.count() > 0) {
        for (int i = 0; i < this->trajPoints.count(); i++) {
            if (this->trajPoints[i].x() < - ((3*this->numVerticalLine) * this->scaleDivX)) {
                this->trajPoints.remove(i);
            } else {
                this->trajPoints[i].setX(this->trajPoints[i].x() - this->period);
            }
        }
    }

    this->trajPoints.append(newPoint);

    this->update();
}

void WidgetDrawPID::clear() {
    this->trajPoints.clear();

    this->clearFlag = true;

    this->update();
}
