#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1450, 1080);

//    auto odomWidget = new QWidget();

    this->globalLayout = new QHBoxLayout();

    QVBoxLayout* drawLayout = new QVBoxLayout();

    this->wd = new WidgetDraw();

    this->wdPID = new WidgetDrawPID();

    this->bar = new QTabBar(this);
    this->bar->addTab("Odometry");
    this->bar->addTab("PID-tune");
    connect(this->bar, SIGNAL(currentChanged(int)), this, SLOT(changedTab(int)));
    drawLayout->addWidget(this->bar);

    drawLayout->addWidget(this->wd);
    drawLayout->addWidget(this->wdPID);

    QHBoxLayout* ctrlButtonsLayout = new QHBoxLayout();

    this->btnStart = new QPushButton("Start");
    btnStart->setToolTip("This is a <b>QPushButton</b> widget");
    btnStart->resize(btnStart->sizeHint());
    ctrlButtonsLayout->addWidget(this->btnStart);

    this->btnStop = new QPushButton("Stop moving");
    ctrlButtonsLayout->addWidget(this->btnStop);

    this->btnNextItr = new QPushButton("Next iteration");
    connect(this->btnNextItr, SIGNAL(clicked()), this->wd, SLOT(clear()));
    connect(this->btnNextItr, SIGNAL(clicked()), this->wdPID, SLOT(clear()));
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

    this->wsPID = new WidgetPIDSettings();
    verticalLayout->addWidget(this->wsPID, 3);

    this->waPID = new WidgetPidAutotune();
    verticalLayout->addWidget(this->waPID, 6);

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
    connect(this->mrc, SIGNAL(sendOdomPoint(OdomDataType)), this->wdPID, SLOT(addTrajPoint(OdomDataType)),  Qt::QueuedConnection);
    connect(this->mrc, SIGNAL(sendFinalPoint(OdomDataType)), this->wc, SLOT(receiveFinalPoint(OdomDataType)),  Qt::QueuedConnection);

    connect(this->btnStart, SIGNAL(clicked()), this->mrc, SLOT(sendStartCmd()),  Qt::QueuedConnection);
    connect(this->btnStop, SIGNAL(clicked()), this->mrc, SLOT(sendStopCmd()),  Qt::QueuedConnection);
    connect(this->btnNextItr, SIGNAL(clicked()), this->mrc, SLOT(sendResetCmd()),  Qt::QueuedConnection);
    connect(this->wrs, SIGNAL(changedRotateDir(bool)), this->mrc, SLOT(changeRotateDir(bool)),  Qt::QueuedConnection);

    qRegisterMetaType<TestData>("TestData");
    connect(this->wts, SIGNAL(sendTestData(TestData)), this->mrc, SLOT(changeTestData(TestData)),  Qt::QueuedConnection);

    qRegisterMetaType<RobotParams>("RobotParams");
    connect(this->wrs, SIGNAL(savedParams(RobotParams)), this->mrc, SLOT(changeRobotParams(RobotParams)),  Qt::QueuedConnection);

    qRegisterMetaType<RobotParams>("PID");
    connect(this->wsPID, SIGNAL(changedPID(PID)), this->mrc, SLOT(sendPIDSetCmd(PID)),  Qt::QueuedConnection);

    connect(this->waPID, SIGNAL(tune()), this->mrc, SLOT(sendPIDTuneCmd()),  Qt::QueuedConnection);
    connect(this->waPID, SIGNAL(sendPID(PID)), this->wsPID, SLOT(setPID(PID)),  Qt::QueuedConnection);
    connect(this->mrc, SIGNAL(sendPIDwihAccuracy(PID, int)), this->waPID, SLOT(setPIDData(PID, int)),  Qt::QueuedConnection);

    this->threadForController->start();
    QMetaObject::invokeMethod(this->mrc, "init");

    globalLayout->addLayout(verticalLayout, 1);

    auto central = new QWidget();
    central->setLayout(globalLayout);

    setCentralWidget(central);

    QTimer::singleShot(100, this->wrs, SLOT(init()));
    QTimer::singleShot(100, this->wts, SLOT(init()));

    this->changedTab(0);
}

void MainWindow::changedTab(int index) {
    switch (index) {
    case 0:
        this->wdPID->setVisible(false);
        this->wsPID->setVisible(false);
        this->waPID->setVisible(false);

        this->wd->setVisible(true);
        this->wts->setVisible(true);
        this->wc->setVisible(true);
        break;
    case 1:
        this->wd->setVisible(false);
        this->wts->setVisible(false);
        this->wc->setVisible(false);

        this->wdPID->setVisible(true);
        this->wsPID->setVisible(true);
        this->waPID->setVisible(true);
        break;
    }
}

MainWindow::~MainWindow()
{
}

