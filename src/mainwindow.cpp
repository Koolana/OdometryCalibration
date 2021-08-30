#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1400, 1080);
    this->globalLayout = new QHBoxLayout(this);

    QVBoxLayout* drawLayout = new QVBoxLayout();

    this->wd = new WidgetDraw();
    drawLayout->addWidget(wd);

    QHBoxLayout* ctrlButtonsLayout = new QHBoxLayout();

    this->btnStart = new QPushButton("Start");
    btnStart->setToolTip("This is a <b>QPushButton</b> widget");
    btnStart->resize(btnStart->sizeHint());
    ctrlButtonsLayout->addWidget(this->btnStart);

    this->btnStop = new QPushButton("Stop moving");
    ctrlButtonsLayout->addWidget(this->btnStop);

    this->btnNextItr = new QPushButton("Next iteration");
    ctrlButtonsLayout->addWidget(this->btnNextItr);

    drawLayout->addLayout(ctrlButtonsLayout);

    globalLayout->addLayout(drawLayout, 5);

    auto verticalLayout = new QVBoxLayout();

    this->wrs = new WidgetRobotSettings();
    verticalLayout->addWidget(this->wrs, 3);

    this->wts = new WidgetTestSettings();
    verticalLayout->addWidget(this->wts, 3);

    this->wc = new WidgetCalculation();
    verticalLayout->addWidget(this->wc, 7);

    globalLayout->addLayout(verticalLayout, 1);

    auto central = new QWidget;
    central->setLayout(globalLayout);

    setCentralWidget(central);
}

MainWindow::~MainWindow()
{
}

