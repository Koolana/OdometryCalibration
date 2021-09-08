#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>

#include "odomdatatype.h"

class ItemData : public QWidget
{
    Q_OBJECT
public:
    explicit ItemData(const OdomDataType& data, QWidget *parent = nullptr);

    void setExperData(const ExperDataType& data);
    const FullData& getData();
    void setIterNum(int num);

private:
    QGridLayout* globalLayout;

    QLabel* labelNumber;
    QLabel* labelRotateDir;
    QLabel* lOdomDataX;
    QLabel* lOdomDataY;
    QLineEdit* leExperDataX;
    QLineEdit* leExperDataY;

    OdomDataType data;

    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

signals:

};

#endif // ITEMDATA_H
