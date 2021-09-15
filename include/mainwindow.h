#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QMetaType>

#include "widgetdraw.h"
#include "widgetrobotsettings.h"
#include "widgettestsettings.h"
#include "widgetcalculation.h"
#include "modelcalibrationalg.h"
#include "modelrobotcontroller.h"

#include "widgetdrawpid.h"
#include "widgetpidsettings.h"

Q_DECLARE_METATYPE(OdomDataType);
Q_DECLARE_METATYPE(TestData);
Q_DECLARE_METATYPE(RobotParams);
Q_DECLARE_METATYPE(PID);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QHBoxLayout* globalLayout;

    QTabBar* bar;

    WidgetDraw* wd;
    WidgetDrawPID* wdPID;

    WidgetRobotSettings* wrs;
    WidgetTestSettings* wts;
    WidgetCalculation* wc;
    WidgetPIDSettings* wsPID;

    ModelCalibrationAlg* mca;
    ModelRobotController* mrc;

    QPushButton* btnStart;
    QPushButton* btnStop;
    QPushButton* btnNextItr;

    QThread* threadForController;

private slots:
    void changedTab(int index);
};
#endif // MAINWINDOW_H
