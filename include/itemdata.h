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
    explicit ItemData(OdomDataType* data, QWidget *parent = nullptr);

    void setExperData(const ExperDataType& data);
    const FullData& getData();

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
    void resizeEvent(QResizeEvent* event);

signals:

};

#endif // ITEMDATA_H
