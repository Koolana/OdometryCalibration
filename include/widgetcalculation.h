#ifndef WIDGETCALCULATION_H
#define WIDGETCALCULATION_H

#include <QGroupBox>
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QStringBuilder>

#include "itemdata.h"

class WidgetCalculation : public QGroupBox
{
    Q_OBJECT
public:
    WidgetCalculation();

private:
    QFileDialog* dialog;

    QGridLayout* globalLayout;

    QListWidget* wDataList;

    QPushButton* btnImport;
    QPushButton* btnExport;
    QPushButton* btnCalc;
    QPushButton* btnClear;

    QLabel* finalKoefsLabel;

    void addDataItemToList(ItemData* item);

private slots:
    void toCalc();
    void importFile();
    void exportToFile();

public slots:
    void updateResult(CalibrationResults& res);
    void receiveFinalPoint(const OdomDataType& data);

signals:
    void sendData(QVector<FullData>&);
};

#endif // WIDGETCALCULATION_H
