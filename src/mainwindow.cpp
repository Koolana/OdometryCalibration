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
    connect(this->wts, SIGNAL(sendTestData(const TestData&)), this->wd, SLOT(changeTest(const TestData&)));
    verticalLayout->addWidget(this->wts, 2);

    this->wc = new WidgetCalculation();
    verticalLayout->addWidget(this->wc, 7);

    this->mca = new ModelCalibrationAlg();
    connect(this->wrs, SIGNAL(savedParams(const RobotParams&)), this->mca, SLOT(changeRobotWidth(const RobotParams&)));
    connect(this->wts, SIGNAL(sendTestData(const TestData&)), this->mca, SLOT(changeTest(const TestData&)));
    connect(this->wc, SIGNAL(sendData(QVector<FullData>&)), this->mca , SLOT(calc(QVector<FullData>&)));
    connect(this->mca, SIGNAL(sendResult(CalibrationResults&)), this->wc, SLOT(updateResult(CalibrationResults&)));

    this->threadForController = new QThread;

    this->mrc = new ModelRobotController();
    this->mrc->moveToThread(this->threadForController);

    qRegisterMetaType<OdomDataType>("OdomDataType");
    connect(this->mrc, SIGNAL(sendOdomPoint(const OdomDataType&)), this->wd, SLOT(addTrajPoint(const OdomDataType&)),  Qt::DirectConnection);

    connect(this->btnStart, SIGNAL(clicked()), this->mrc, SLOT(sendStartCmd()),  Qt::DirectConnection);
    connect(this->btnStop, SIGNAL(clicked()), this->mrc, SLOT(sendStopCmd()),  Qt::DirectConnection);
    connect(this->btnNextItr, SIGNAL(clicked()), this->mrc, SLOT(sendResetCmd()),  Qt::DirectConnection);
    connect(this->wrs, SIGNAL(changedRotateDir(bool)), this->mrc, SLOT(changeRotateDir(bool)),  Qt::DirectConnection);

    qRegisterMetaType<TestData>("TestData");
    connect(this->wts, SIGNAL(sendTestData(const TestData&)), this->mrc, SLOT(changeTestData(const TestData&)),  Qt::DirectConnection);

    qRegisterMetaType<RobotParams>("RobotParams");
    connect(this->wrs, SIGNAL(savedParams(const RobotParams&)), this->mrc, SLOT(changeRobotParams(const RobotParams&)),  Qt::DirectConnection);

    this->threadForController->start();
    QMetaObject::invokeMethod(this->mrc, "init");

    globalLayout->addLayout(verticalLayout, 1);

    auto central = new QWidget;
    central->setLayout(globalLayout);

    setCentralWidget(central);
}

MainWindow::~MainWindow()
{
}

