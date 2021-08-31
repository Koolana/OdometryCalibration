#ifndef WIDGETCALCULATION_H
#define WIDGETCALCULATION_H

#include <QGroupBox>
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>

#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

#include "itemdata.h"

class WidgetCalculation : public QGroupBox
{
    Q_OBJECT
public:
    WidgetCalculation();

public slots:
    void updateResult(CalibrationResults& res);

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
    void receiveFinalPoint(OdomDataType& data);

private slots:
    void toCalc();
    void importFile();
    void exportToFile();

signals:
    void sendData(QVector<FullData>&);
};

#endif // WIDGETCALCULATION_H
