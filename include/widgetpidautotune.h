#ifndef WIDGETPIDAUTOTUNE_H
#define WIDGETPIDAUTOTUNE_H

#include <QGroupBox>
#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "odomdatatype.h"

class WidgetPidAutotune : public QGroupBox
{
    Q_OBJECT
public:
    WidgetPidAutotune();

private:
    QGridLayout* globalLayout;

    QLabel* lAccuracyIndicate;
    QLabel* lPValue;
    QLabel* lIValue;
    QLabel* lDValue;

    QPushButton* btnTune;
    QPushButton* btnAccept;

private slots:
    void accept();

public slots:
    void setPIDData(const PID& pid, int accuracy);

signals:
    void tune();
    void sendPID(const PID& pid);
};

#endif // WIDGETPIDAUTOTUNE_H
