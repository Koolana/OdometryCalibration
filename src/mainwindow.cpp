#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1450, 1080);
    this->globalLayout = new QHBoxLayout();

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
    connect(this->btnNextItr, SIGNAL(clicked()), this->wd, SLOT(clear()));
    ctrlButtonsLayout->addWidget(this->btnNextItr);

    drawLayout->addLayout(ctrlButtonsLayout);

    globalLayout->addLayout(drawLayout, 5);

    auto verticalLayout = new QVBoxLayout();

    this->wrs = new WidgetRobotSettings();
    connect(this->wrs, SIGNAL(changedRotateDir(bool)), this->wd, SLOT(changeRotateDir(bool)));
    verticalLayout->addWidget(this->wrs, 2);

    this->wts = new WidgetTestSettings();
    connect(this->wts, SIGNAL(sendTestData(TestData)), this->wd, SLOT(changeTest(TestData)));
    verticalLayout->addWidget(this->wts, 2);

    this->wc = new WidgetCalculation();
    verticalLayout->addWidget(this->wc, 7);

    this->mca = new ModelCalibrationAlg();
    connect(this->wrs, SIGNAL(savedParams(RobotParams)), this->mca, SLOT(changeRobotWidth(RobotParams)),  Qt::DirectConnection);
    connect(this->wts, SIGNAL(sendTestData(TestData)), this->mca, SLOT(changeTest(TestData)),  Qt::DirectConnection);
    connect(this->wc, SIGNAL(sendData(QVector<FullData>&)), this->mca , SLOT(calc(QVector<FullData>&)),  Qt::DirectConnection);
    connect(this->mca, SIGNAL(sendResult(CalibrationResults&)), this->wc, SLOT(updateResult(CalibrationResults&)),  Qt::DirectConnection);

    this->threadForController = new QThread;

    this->mrc = new ModelRobotController();
    this->mrc->moveToThread(this->threadForController);

    connect(this->wrs, SIGNAL(sendDevicePath(QString)), this->mrc, SLOT(openDevice(QString)),  Qt::QueuedConnection);
    connect(this->mrc, SIGNAL(connected(bool)), this->wrs, SLOT(displayConnectInfo(bool)),  Qt::QueuedConnection);

    qRegisterMetaType<OdomDataType>("OdomDataType");
    connect(this->mrc, SIGNAL(sendOdomPoint(OdomDataType)), this->wd, SLOT(addTrajPoint(OdomDataType)),  Qt::QueuedConnection);
    connect(this->mrc, SIGNAL(sendFinalPoint(OdomDataType)), this->wc, SLOT(receiveFinalPoint(OdomDataType)),  Qt::QueuedConnection);

    connect(this->btnStart, SIGNAL(clicked()), this->mrc, SLOT(sendStartCmd()),  Qt::QueuedConnection);
    connect(this->btnStop, SIGNAL(clicked()), this->mrc, SLOT(sendStopCmd()),  Qt::QueuedConnection);
    connect(this->btnNextItr, SIGNAL(clicked()), this->mrc, SLOT(sendResetCmd()),  Qt::QueuedConnection);
    connect(this->wrs, SIGNAL(changedRotateDir(bool)), this->mrc, SLOT(changeRotateDir(bool)),  Qt::QueuedConnection);

    qRegisterMetaType<TestData>("TestData");
    connect(this->wts, SIGNAL(sendTestData(TestData)), this->mrc, SLOT(changeTestData(TestData)),  Qt::QueuedConnection);

    qRegisterMetaType<RobotParams>("RobotParams");
    connect(this->wrs, SIGNAL(savedParams(RobotParams)), this->mrc, SLOT(changeRobotParams(RobotParams)),  Qt::QueuedConnection);

    this->threadForController->start();
    QMetaObject::invokeMethod(this->mrc, "init");

    globalLayout->addLayout(verticalLayout, 1);

    auto central = new QWidget;
    central->setLayout(globalLayout);

    setCentralWidget(central);

    QTimer::singleShot(100, this->wrs, SLOT(init()));
    QTimer::singleShot(100, this->wts, SLOT(init()));
}

MainWindow::~MainWindow()
{
}

