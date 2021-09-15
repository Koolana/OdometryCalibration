#include "include/widgetdrawpid.h"

WidgetDrawPID::WidgetDrawPID(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());

    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
}

void WidgetDrawPID::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter qp(this);
    this->drawScale(qp);
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
        if (vl == int(this->numVerticalLine / 2)) {
            pen.setWidth(2);
            qp.setPen(pen);
        } else {
            pen.setWidth(1);
            qp.setPen(pen);
        }

        qp.rotate(-90);
        qp.drawText(-this->size().height() + 10,
                     this->size().width() / this->numVerticalLine * vl - 2,
                     QString::number(-(this->numVerticalLine / 2 - vl) * this->scaleDiv) + " m");
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
                     QString::number((this->numHorizontalLine / 2 - hl) * this->scaleDiv) + " m");
        qp.drawLine(0,
                     float(this->size().height()) / float(this->numHorizontalLine) * hl,
                     this->size().width(),
                     float(this->size().height()) / float(this->numHorizontalLine) * hl);
    }

    qp.restore();
}
