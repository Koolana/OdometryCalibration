#include "../include/widgettestsettings.h"

#include <QDebug>

WidgetTestSettings::WidgetTestSettings()
{
    this->setTitle("Test settings");

    this->globalLayout = new QGridLayout();

    this->labelParam = new QLabel("Length:");
    this->labelParam->setAlignment(Qt::AlignBottom);
    this->globalLayout->addWidget(this->labelParam, 0, 0);

    this->leSize = new QLineEdit("1");
    connect(this->leSize, SIGNAL(editingFinished()), this, SLOT(choosedTest()));
    this->globalLayout->addWidget(this->leSize, 1, 0);

    auto radiobutton = new QRadioButton("Line");
    radiobutton->setChecked(true);
    connect(radiobutton, SIGNAL(clicked()), this, SLOT(changedTest()));
    this->globalLayout->addWidget(radiobutton, 2, 0);

    radiobutton = new QRadioButton("Square");
    connect(radiobutton, SIGNAL(clicked()), this, SLOT(changedTest()));
    this->globalLayout->addWidget(radiobutton, 3, 0);

    radiobutton = new QRadioButton("Сircle");
    connect(radiobutton, SIGNAL(clicked()), this, SLOT(changedTest()));
    this->globalLayout->addWidget(radiobutton, 4, 0);

    this->leNumIter = new QLineEdit("1");
//    self.leNumIter.editingFinished.connect(self.maxIterationChanged)
    this->globalLayout->addWidget(this->leNumIter, 4, 1);

    this->setLayout(this->globalLayout);
}

void WidgetTestSettings::changedTest() {
    QButtonGroup group;
    QList<QRadioButton*> allButtons = this->findChildren<QRadioButton*>();

    for(int i = 0; i < allButtons.size(); ++i)
    {
        group.addButton(allButtons[i],i);
    }

    this->currTestType = group.checkedId();

    switch (this->currTestType) {
    case 0:
        this->labelParam->setText("Length:");
        break;
    case 1:
        this->labelParam->setText("Side length:");
        break;
    case 2:
        this->labelParam->setText("Radius:");
        break;
    }

    this->choosedTest();
}

void WidgetTestSettings::choosedTest() {
    TestData td;
    td.numIter = 1;
    td.size = this->leSize->text().toFloat();
    td.typeTest = (Tests)this->currTestType;

    emit sendTestData(td);
}
