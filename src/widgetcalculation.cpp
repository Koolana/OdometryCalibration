#include "../include/widgetcalculation.h"

#include <QDebug>

WidgetCalculation::WidgetCalculation()
{
    this->setTitle("Experiment information");

    this->dialog = new QFileDialog(this);
    this->dialog->setFileMode(QFileDialog::AnyFile);
    this->dialog->setViewMode(QFileDialog::Detail);

    this->globalLayout = new QGridLayout();

    auto label = new QLabel("Odometry\ndata");
    label->setAlignment(Qt::AlignCenter);
    this->globalLayout->addWidget(label, 0, 0);

    label = new QLabel("Real\ndata");
    label->setAlignment(Qt::AlignCenter);
    globalLayout->addWidget(label, 0, 1);

    this->wDataList = new QListWidget();
    this->globalLayout->addWidget(this->wDataList, 1, 0, 4, 2);

    this->btnImport = new QPushButton("Import");
    connect(this->btnImport, SIGNAL(clicked()), this, SLOT(importFile()));
    this->globalLayout->addWidget(this->btnImport, 5, 0);

    this->btnExport = new QPushButton("Export");
    connect(this->btnExport, SIGNAL(clicked()), this, SLOT(exportToFile()));
    this->globalLayout->addWidget(this->btnExport, 5, 1);

    this->btnCalc = new QPushButton("Calculate!");
    this->btnCalc->setToolTip("This is a <b>QPushButton</b> widget");
    this->btnCalc->resize(this->btnCalc->sizeHint());
    connect(this->btnCalc, SIGNAL(clicked()), this, SLOT(toCalc()));
    this->globalLayout->addWidget(this->btnCalc, 6, 0, 1, 2);

    this->btnClear = new QPushButton("Clear");
    connect(this->btnClear, SIGNAL(clicked()), this->wDataList, SLOT(clear()));
    this->globalLayout->addWidget(this->btnClear, 7, 0, 1, 2);

    this->finalKoefsLabel = new QLabel();
    this->globalLayout->addWidget(this->finalKoefsLabel, 8, 0, 4, 2);

    this->setLayout(this->globalLayout);
}

void WidgetCalculation::importFile() {
    QString filePath = this->dialog->getOpenFileName(this, "Choose a data-file",
                                              QDir::homePath(), "Data files (*.csv)", nullptr,
                                              QFileDialog::DontUseNativeDialog);

    QFile file(filePath);
    if (!file.exists() or !file.open(QIODevice::ReadOnly)) {
        return;
    }

    QStringList lineList = QString(file.readAll()).split('\n');

    for (int i = 0; i < lineList.count(); i++) {
        QStringList values = lineList.at(i).split(',');

        if (values.count() != 5) {
            continue;
        }

        auto testDataOdom = new OdomDataType();
        testDataOdom->x = values.at(1).toFloat();
        testDataOdom->y = values.at(2).toFloat();
        testDataOdom->isClockwise = (values.at(0) == "True" ? true : false);
        testDataOdom->numIter = i + 1;

        auto testDataExper = new ExperDataType();
        testDataExper->x = values.at(3).toFloat();
        testDataExper->y = values.at(4).toFloat();

        auto itemData = new ItemData(testDataOdom);
        itemData->setExperData(*testDataExper);

        this->addDataItemToList(itemData);
    }

    file.close();
}

void WidgetCalculation::exportToFile() {
    qDebug() << "Export";
}

void WidgetCalculation::addDataItemToList(ItemData* item) {
    auto lwi = new QListWidgetItem(this->wDataList);
    lwi->setFlags(Qt::NoItemFlags);
    lwi->setSizeHint(item->sizeHint());

    this->wDataList->addItem(lwi);
    this->wDataList->setItemWidget(lwi, item);
//    qDebug() << QString("New odom point added");
}

void WidgetCalculation::receiveFinalPoint(OdomDataType& data) {
    auto itemData = new ItemData(&data);
    this->addDataItemToList(itemData);

    this->update();
}

void WidgetCalculation::toCalc() {
    QVector<FullData> dataList;

    for(int i = 0; i < this->wDataList->count(); i++)
    {
        QListWidgetItem* item = this->wDataList->item(i);
        dataList.append(((ItemData*)this->wDataList->itemWidget(item))->getData());
    }

    emit sendData(dataList);
}

void WidgetCalculation::updateResult(CalibrationResults& res) {
    switch (res.typeTest) {
    case Tests::LINE:
       this->finalKoefsLabel->setText("Results:\nCl = " + QString::number(res.CL) + "\n"
                                      + "Cr = " + QString::number(res.CR));
       break;
    case Tests::SQUARE:
        this->finalKoefsLabel->setText("Results:\nb_actual = " + QString::number(res.b) + "\n"
                                       + "Cl = " + QString::number(res.CL) + "\n"
                                       + "Cr = " + QString::number(res.CR));
        break;
    case Tests::CIRCLE:
        this->finalKoefsLabel->setText("Calculation algorithm\nnot found!");
        break;
    case Tests::ERROR:
        this->finalKoefsLabel->setText("An error occurred\nwhile calculating!");
        break;
    }
}
