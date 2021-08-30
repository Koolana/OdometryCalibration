#include "mainwindow.h"

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

    globalLayout->addLayout(drawLayout);

    auto central = new QWidget;
    central->setLayout(globalLayout);

    setCentralWidget(central);
}

MainWindow::~MainWindow()
{
}

