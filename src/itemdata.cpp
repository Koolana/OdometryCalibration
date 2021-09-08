#include "../include/itemdata.h"

ItemData::ItemData(const OdomDataType& data, QWidget *parent) : QWidget(parent)
{
    this->data = data;

    this->globalLayout = new QGridLayout();
    this->globalLayout->setSpacing(0);
    this->globalLayout->setContentsMargins(5, 5, 0, 5);

    this->labelNumber = new QLabel(QString::number(this->data.numIter));
    this->globalLayout->addWidget(this->labelNumber, 0, 0, 3, 0);

    this->labelRotateDir = new QLabel(this->data.isClockwise ? "CW" : "CCW");
    this->labelRotateDir->setFont(QFont("Helvetica [Cronyx]", 5));
    this->labelRotateDir->setAlignment(Qt::AlignBottom);
    this->globalLayout->addWidget(this->labelRotateDir, 2, 0, 1, 0);

    auto titleX = new QLabel("x");
    titleX->setAlignment(Qt::AlignCenter);
    this->globalLayout->addWidget(titleX, 0, 1, 1, 1);

    auto titleY = new QLabel("y");
    titleY->setAlignment(Qt::AlignCenter);
    this->globalLayout->addWidget(titleY, 2, 1, 1, 1);

    this->lOdomDataX = new QLabel(QString::number(this->data.x));
    this->lOdomDataX->setAlignment(Qt::AlignCenter);
    this->globalLayout->addWidget(this->lOdomDataX, 0, 2, 1, 4);

    this->lOdomDataY = new QLabel(QString::number(this->data.y));
    this->lOdomDataY->setAlignment(Qt::AlignCenter);
    this->globalLayout->addWidget(this->lOdomDataY, 2, 2, 1, 4);

    this->leExperDataX = new QLineEdit();
    this->globalLayout->addWidget(this->leExperDataX, 0, 6, 1, 4);

    this->leExperDataY = new QLineEdit();
    this->globalLayout->addWidget(this->leExperDataY, 2, 6, 1, 4);

    auto l = new QLabel();
    l->setFixedHeight(8);
    globalLayout->addWidget(l, 1, 0, 1, 9);

    this->setLayout(this->globalLayout);
}

void ItemData::setExperData(const ExperDataType& data) {
    this->leExperDataX->setText(QString::number(data.x));
    this->leExperDataY->setText(QString::number(data.y));
}

const FullData& ItemData::getData() {
    FullData* data = new FullData();

    data->odom = this->data;
    data->exper.x = this->leExperDataX->text().toFloat();
    data->exper.y = this->leExperDataY->text().toFloat();

    return *data;
}

void ItemData::setIterNum(int num) {
    this->labelNumber->setText(QString::number(num));
}

void ItemData::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter qp(this);
    qp.drawLine(this->width() / 9 - 1, 0, this->width() / 9 - 1, this->height());
    qp.drawLine(this->width() / 9 + 1, 0, this->width() / 9 + 1, this->height());

    qp.drawLine(2 * this->width() / 9 - 1, 0, 2 * this->width() / 9 - 1, this->height());
    qp.drawLine(2 * this->width() / 9 + 1, 0, 2 * this->width() / 9 + 1, this->height());

    qp.drawLine(5 * this->width() / 9 + 1, 0, 5 * this->width() / 9 + 1, this->height());
    qp.drawLine(5 * this->width() / 9 - 1, 0, 5 * this->width() / 9 - 1, this->height());

    qp.drawLine(this->width() / 9, this->height() / 2, this->width(), this->height() / 2);

    qp.drawLine(0, this->height() - 1, this->width(), this->height() - 1);
    qp.drawLine(0, this->height() - 2, this->width(), this->height() - 2);
}

void ItemData::resizeEvent(QResizeEvent* event) {
    Q_UNUSED(event);

    this->leExperDataX->setFixedWidth(3 * this->width() / 9);
    this->leExperDataY->setFixedWidth(3 * this->width() / 9);

    this->update();
}
