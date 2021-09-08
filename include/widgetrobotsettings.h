#ifndef WIDGETROBOTSETTINGS_H
#define WIDGETROBOTSETTINGS_H

#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>

#include "odomdatatype.h"

class WidgetRobotSettings : public QGroupBox
{
    Q_OBJECT
public:
    WidgetRobotSettings();

private:
    QGridLayout* globalLayout;

    QPushButton* btnRotate;
    QPushButton* btnSave;

    QLineEdit* leWidth;
    QLineEdit* leLinSpeed;
    QLineEdit* leAngSpeed;

    const QIcon cwCircleArrow = QIcon(":/img/img/cw.png");
    const QIcon ccwCircleArrow = QIcon(":/img/img/ccw.png");
    bool isCW = true;

private slots:
    void saveParams();
    void swapRotateDirection();

signals:
    void changedRotateDir(bool);
    void savedParams(const RobotParams&);
};

#endif // WIDGETROBOTSETTINGS_H
