#ifndef WIDGETTESTSETTINGS_H
#define WIDGETTESTSETTINGS_H

#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QString>
#include "odomdatatype.h"

class WidgetTestSettings : public QGroupBox
{
    Q_OBJECT
public:
    WidgetTestSettings();

private:
    QGridLayout* globalLayout;

    QLabel* labelParam;

    QLineEdit* leSize;
    QLineEdit* leNumIter;

    int currTestType = 0;

private slots:
    void changedTest();
    void choosedTest();

public slots:
    void init();

signals:
    void sendTestData(const TestData&);
};

#endif // WIDGETTESTSETTINGS_H
