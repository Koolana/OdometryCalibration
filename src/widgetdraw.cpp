#include "include/widgetdraw.h"

#include <QDebug>

WidgetDraw::WidgetDraw(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());

    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    this->drawRobotWidth = this->robotLenX / this->scaleDiv * this->size().width() / this->numVerticalLine;
    this->drawRobotHeight = this->robotLenY / this->scaleDiv * this->size().height() / this->numHorizontalLine;
}

void WidgetDraw::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter qp(this);
    this->drawScale(qp);
    this->drawRobot(qp);
    this->drawTarget(qp);
    this->drawTrajectory(qp);
    this->drawCurrentRobotInfo(qp);
}

void WidgetDraw::drawScale(QPainter& qp) {
    qp.save();

    QPen penL(QColor(220, 220, 220, 255), 1, Qt::SolidLine);
    QPen penT(QColor(0, 0, 0, 255), 1, Qt::SolidLine);
//    pen.setWidth(1);

    for (int vl = 1; vl < this->numVerticalLine; vl++) {
        if (vl == int(this->numVerticalLine / 2)) {
            penL.setWidth(2);
        } else {
            penL.setWidth(1);
        }

        qp.rotate(-90);
        qp.setPen(penT);
        qp.drawText(-this->size().height() + 10,
                     this->size().width() / this->numVerticalLine * vl - 2,
                     QString::number(-(this->numVerticalLine / 2 - vl) * this->scaleDiv) + " m");
        qp.rotate(90);

        qp.setPen(penL);
        qp.drawLine(float(this->size().width()) / float(this->numVerticalLine) * vl,
                     0,
                     float(this->size().width()) / float(this->numVerticalLine) * vl,
                     this->size().height());
    }

    for (int hl = 1; hl < this->numHorizontalLine; hl++) {
        if (hl == int(this->numHorizontalLine / 2)) {
            penL.setWidth(2);
        } else {
            penL.setWidth(1);
        }

        qp.setPen(penT);
        qp.drawText(10,
                     this->size().height() / this->numHorizontalLine * hl - 2,
                     QString::number((this->numHorizontalLine / 2 - hl) * this->scaleDiv) + " m");

        qp.setPen(penL);
        qp.drawLine(0,
                     float(this->size().height()) / float(this->numHorizontalLine) * hl,
                     this->size().width(),
                     float(this->size().height()) / float(this->numHorizontalLine) * hl);
    }

    qp.restore();
}

void WidgetDraw::drawRobot(QPainter& qp) {
    qp.save();
    qp.setBrush(QColor(0, 0, 0, 100));
//    qDebug() << this->robotPos.x() / this->scaleDiv * this->size().width() / this->numVerticalLine + this->size().width() / 2;
    qp.translate(this->robotPos.x / this->scaleDiv * this->size().width() / this->numVerticalLine + this->size().width() / 2,
                -this->robotPos.y / this->scaleDiv * this->size().height() / this->numHorizontalLine + this->size().height() / 2);
    qp.rotate(-robotPos.th * 180 / M_PI);

    qp.drawRect(-this->drawRobotWidth / 2, -this->drawRobotHeight / 2,
                this->drawRobotWidth, this->drawRobotHeight);

    QPen pen(QColor(255, 20, 20, 255), 2, Qt::SolidLine);
    qp.setPen(pen);
    qp.drawLine(0, 0, 100, 0);
    qp.drawLine(100, 0, 90, -4);
    qp.drawLine(100, 0, 90, 4);

    qp.restore();
}

void WidgetDraw::drawTarget(QPainter& qp) {
    qp.save();

    QPen pen(QColor(0, 150, 0, 255), 2, Qt::DotLine);
    qp.setPen(pen);

    switch (this->currTest.typeTest) {
    case Tests::LINE:
        qp.drawLine(0 + this->size().width() / 2,
                    0 + this->size().height() / 2,
                    this->currTest.size / this->scaleDiv * this->size().width() / this->numVerticalLine + this->size().width() / 2,
                    0 + this->size().height() / 2);
        break;
    case Tests::SQUARE:
        qp.drawRect(0 + this->size().width() / 2,
                    ((this->isCW ? 0 : -this->currTest.size) / this->scaleDiv * this->size().height() / this->numHorizontalLine) + this->size().height() / 2,
                    this->currTest.size / this->scaleDiv * this->size().width() / this->numVerticalLine,
                    this->currTest.size / this->scaleDiv * this->size().height() / this->numHorizontalLine);
        break;
    case Tests::CIRCLE:
        qp.drawEllipse(-2 * this->currTest.size / this->scaleDiv * this->size().width() / this->numVerticalLine / 2 + this->size().width() / 2,
                       ((this->isCW ? 0 : -2) * this->currTest.size / this->scaleDiv * this->size().height() / this->numHorizontalLine) + this->size().height() / 2,
                       2 * this->currTest.size / this->scaleDiv * this->size().width() / this->numVerticalLine,
                       2 * this->currTest.size / this->scaleDiv * this->size().height() / this->numHorizontalLine);
        break;
    case Tests::ERROR:
        break;
    }

    qp.restore();
}

void WidgetDraw::drawTrajectory(QPainter& qp) {
    if (this->trajPoints.count() < 2) {
        return;
    }

    qp.save();

    QPen pen(QColor(0, 0, 255), 2, Qt::SolidLine);

    OdomDataType prevPoint = this->trajPoints.first();

    for (int i = 1; i < this->trajPoints.count(); i++) {
        const OdomDataType* currPoint = &this->trajPoints.at(i);

        pen.setColor(this->trajColors.at(currPoint->numIter));
        qp.setPen(pen);

        qp.drawLine(prevPoint.x / this->scaleDiv * this->size().width() / this->numVerticalLine + this->size().width() / 2,
        -prevPoint.y / this->scaleDiv * this->size().height() / this->numHorizontalLine + this->size().height() / 2,
        currPoint->x / this->scaleDiv * this->size().width() / this->numVerticalLine + this->size().width() / 2,
        -currPoint->y / this->scaleDiv * this->size().height() / this->numHorizontalLine + this->size().height() / 2);

        prevPoint = *currPoint;
    }

    qp.restore();
}

void WidgetDraw::drawCurrentRobotInfo(QPainter& qp) {
    qp.save();

    qp.setBrush(QColor(255, 255, 255, 255));
    qp.setPen(QPen(QColor(0, 0, 0, 0)));

    int shiftFrame = 160;

    qp.drawRect(this->size().width() - shiftFrame, 0, shiftFrame, 5*20 + 8);

    qp.setPen(QPen(QColor(0, 0, 0, 255)));
    qp.setFont(QFont("Helvetica [Cronyx]", 12));

    qp.drawText(this->size().width() - shiftFrame + 10, 1 * 20, "x:");
    qp.drawText(this->size().width() - shiftFrame + 40, 1 * 20, QString("%1 m").arg(QString::number(this->robotPos.x, 'f', 2), 6, QChar(' ')));

    qp.drawText(this->size().width() - shiftFrame + 10, 2 * 20, "y:");
    qp.drawText(this->size().width() - shiftFrame + 40, 2 * 20, QString("%1 m").arg(QString::number(this->robotPos.y, 'f', 2), 6, QChar(' ')));

    qp.drawText(this->size().width() - shiftFrame + 10, 3 * 20, "th:");
    qp.drawText(this->size().width() - shiftFrame + 40, 3 * 20, QString("%1 rad").arg(QString::number(this->robotPos.th, 'f', 2), 6, QChar(' ')));

    qp.drawText(this->size().width() - shiftFrame + 10, 4 * 20, "vx:");
    qp.drawText(this->size().width() - shiftFrame + 40, 4 * 20, QString("%1 m/sec").arg(QString::number(this->robotPos.vx, 'f', 2), 6, QChar(' ')));

    qp.drawText(this->size().width() - shiftFrame + 10, 5 * 20, "vth:");
    qp.drawText(this->size().width() - shiftFrame + 40, 5 * 20, QString("%1 rad/sec").arg(QString::number(this->robotPos.vth, 'f', 2), 6, QChar(' ')));

    qp.restore();
}

void WidgetDraw::resizeEvent(QResizeEvent* event) {
    Q_UNUSED(event);

    if (this->size().width() > this->size().height()) {
        this->numHorizontalLine = 10;

        this->numVerticalLine = this->size().height() ? int(10 * this->size().width() / this->size().height()) : 0;
        this->numVerticalLine += this->numVerticalLine % 2 == 0 ? 0 : 1;
    } else {
        this->numHorizontalLine = this->size().width() ? int(10 * this->size().height() / this->size().width()) : 0;
        this->numHorizontalLine += this->numHorizontalLine % 2 == 0 ? 0 : 1;

        this->numVerticalLine = 10;
    }

    this->drawRobotWidth = this->robotLenX / this->scaleDiv * this->size().width() / this->numVerticalLine;
    this->drawRobotHeight = this->robotLenY / this->scaleDiv * this->size().height() / this->numHorizontalLine;

    this->update();
}

void WidgetDraw::changeTest(const TestData& data) {
    this->currTest = data;

    this->update();
}

void WidgetDraw::changeRotateDir(bool isCW) {
    this->isCW = isCW;

    this->update();
}

void WidgetDraw::addTrajPoint(const OdomDataType& data) {
    if (this->clearFlag) {
        this->clearFlag = false;
        return;
    }

    this->robotPos = data;

    if (this->trajPoints.count() > 0) {
        if (qSqrt(qPow(this->trajPoints.last().x - data.x, 2) + qPow(this->trajPoints.last().y - data.y, 2)) > 0.02) {
            this->trajPoints.append(data);
        }
    } else {
        this->trajPoints.append(data);
    }

    if (this->trajColors.count() == 0) {
        this->trajColors.append(QColor(0, 0, 255));
    } else {
        if (this->trajColors.count() == data.numIter) {
            this->trajColors.append(QColor(qrand() % ((255 + 1) - 0) + 0, qrand() % ((255 + 1) - 0) + 0, qrand() % ((255 + 1) - 0) + 0));
        }
    }

    this->update();
}

void WidgetDraw::clear() {
    this->trajPoints.clear();
    this->trajColors.clear();

    this->clearFlag = true;

    this->update();
}
