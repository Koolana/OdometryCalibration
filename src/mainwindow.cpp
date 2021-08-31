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
    connect(this->wrs, SIGNAL(changedRotateDir(bool)), this->wd, SLOT(changeRotateDir(bool)));
    verticalLayout->addWidget(this->wrs, 2);

    this->wts = new WidgetTestSettings();
    connect(this->wts, SIGNAL(sendTestData(TestData&)), this->wd, SLOT(changeTest(TestData&)));
    verticalLayout->addWidget(this->wts, 2);

    this->wc = new WidgetCalculation();
    verticalLayout->addWidget(this->wc, 7);

    this->mca = new ModelCalibrationAlg();
    connect(this->wrs, SIGNAL(savedParams(RobotParams&)), this->mca, SLOT(changeRobotWidth(RobotParams&)));
    connect(this->wts, SIGNAL(sendTestData(TestData&)), this->mca, SLOT(changeTest(TestData&)));
    connect(this->wc, SIGNAL(sendData(QVector<FullData>&)), this->mca, SLOT(calc(QVector<FullData>&)));
    connect(this->mca, SIGNAL(sendResult(CalibrationResults&)), this->wc, SLOT(updateResult(CalibrationResults&)));

    globalLayout->addLayout(verticalLayout, 1);

    auto central = new QWidget;
    central->setLayout(globalLayout);

    setCentralWidget(central);
}

MainWindow::~MainWindow()
{
}

