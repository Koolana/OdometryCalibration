#ifndef WIDGETPIDSETTINGS_H
#define WIDGETPIDSETTINGS_H

#include <QGroupBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QObject>

#include "odomdatatype.h"

class WidgetPIDSettings : public QGroupBox
{
    Q_OBJECT
public:
    WidgetPIDSettings();

private:
    QGridLayout* globalLayout;

    QLineEdit* leP;
    QLineEdit* leI;
    QLineEdit* leD;

    QPushButton* btnApply;

private slots:
    void sendPID();

public slots:
    void setPID(const PID&);

signals:
    void changedPID(const PID&);
};

#endif // WIDGETPIDSETTINGS_H
