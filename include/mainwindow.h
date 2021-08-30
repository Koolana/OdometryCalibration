#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "widgetdraw.h"
#include "widgetrobotsettings.h"
#include "widgettestsettings.h"
#include "widgetcalculation.h"

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

    QPushButton* btnStart;
    QPushButton* btnStop;
    QPushButton* btnNextItr;
};
#endif // MAINWINDOW_H
