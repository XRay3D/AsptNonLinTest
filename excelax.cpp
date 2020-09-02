#include "excelax.h"
#include "mytablemodel.h"
#include <QApplication>
#include <QAxObject>
#include <QDateTime>

ExcelAx::ExcelAx()
{
}

void ExcelAx::loadFile(const QString& fileName, MyTableModel* m_model)
{
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible(bool)", true);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileName);
    QAxObject* sheets = workbook->querySubObject("Worksheets");
    QVariant value;
    if (workbooks->dynamicCall("Count()").toInt()) {
        QAxObject* sheet = sheets->querySubObject("Item(int)", 1);
        for (int devCh = 0; devCh < RowCount; ++devCh) {
            m_model->clearData(devCh);
            for (int adcCh = 0; adcCh < 2; ++adcCh) {
                for (int resCh = 0; resCh < 3; ++resCh) {
                    if (!adcCh) {
                        QAxObject* cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 4);
                        value = cell->dynamicCall("Value()");
                    } else {
                        QAxObject* cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 7);
                        value = cell->dynamicCall("Value()");
                    }
                    m_model->addData(devCh, adcCh * 3 + resCh, value.toDouble());
                }
            }
        }
        workbook->dynamicCall("Close()");
    }
    excel->dynamicCall("Quit()");
    excel->deleteLater();
}

void ExcelAx::saveFile(QString& fileName, const QString& asptNum, const QString& fio, MyTableModel* m_model)
{
    bool newFile = false;
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible(bool)", true);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    if (fileName.isEmpty()) {
        fileName = qApp->applicationDirPath() + "/blank.xlsx";
        newFile = true;
    }
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileName);
    QAxObject* sheets = workbook->querySubObject("Worksheets");
    if (workbooks->dynamicCall("Count()").toInt()) {
        QAxObject* sheet = sheets->querySubObject("Item(int)", 1);
        QAxObject* cell;
        if (newFile) {
            cell = sheet->querySubObject("Cells(int,int)", 55, 4);
            cell->dynamicCall("SetValue2(QVariant)", QDateTime::currentDateTime());
        }
        cell = sheet->querySubObject("Cells(int,int)", 2, 5);
        cell->dynamicCall("SetValue2(QVariant)", asptNum);
        cell = sheet->querySubObject("Cells(int,int)", 57, 4);
        cell->dynamicCall("SetValue2(QVariant)", fio);
        cell = sheet->querySubObject("Cells(int,int)", 56, 4);
        cell->dynamicCall("SetValue2(QVariant)", QDateTime::currentDateTime());
        for (int devCh = 0; devCh < RowCount; ++devCh) {
            for (int adcCh = 0; adcCh < 2; ++adcCh) {
                for (int resCh = 0; resCh < 3; ++resCh) {
                    QAxObject* cell;
                    if (!adcCh) {
                        cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 4);
                    } else {
                        cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 7);
                    }
                    cell->dynamicCall("SetValue2(QVariant)", m_model->getData(devCh).average(adcCh * 3 + resCh));
                }
            }
        }

        if (newFile)
            workbook->dynamicCall("SaveAs(const QVariant&)", QVariant(QString(fileName).replace('/', '\\')));
        else
            workbook->dynamicCall("Save()");

        workbook->dynamicCall("Close()");
    }
    excel->dynamicCall("Quit()");
    excel->deleteLater();
}

void ExcelAx::printFile(const QString& fileName)
{
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible(bool)", true);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileName);
    QAxObject* sheets = workbook->querySubObject("Worksheets");
    if (workbooks->dynamicCall("Count()").toInt()) {
        QAxObject* sheet = sheets->querySubObject("Item(int)", 1);
        sheet->dynamicCall("PrintOut(QVariant, QVariant, QVariant)", 1, 1, 1);
        workbook->dynamicCall("Close()");
    }
    excel->dynamicCall("Quit()");
    excel->deleteLater();
}
