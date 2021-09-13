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

Q_DECLARE_METATYPE(OdomDataType);
Q_DECLARE_METATYPE(TestData);
Q_DECLARE_METATYPE(RobotParams);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QHBoxLayout* globalLayout;

    WidgetDraw* wd;
    WidgetRobotSettings* wrs;
    WidgetTestSettings* wts;
    WidgetCalculation* wc;

    ModelCalibrationAlg* mca;
    ModelRobotController* mrc;

    QPushButton* btnStart;
    QPushButton* btnStop;
    QPushButton* btnNextItr;

    QThread* threadForController;
};
#endif // MAINWINDOW_H
