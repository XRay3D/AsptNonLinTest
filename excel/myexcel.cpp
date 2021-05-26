#include "myexcel.h"
#include "excel.h"
#include "mytablemodel.h"
//#include <QApplication>
//#include <QAxObject>
//#include <QDateTime>
//#include <QFileInfo>

MyExcel::MyExcel() {
    system("taskkill /f /im excel.exe");
    excel = new Excel::Application;
    excel->SetVisible(true);
}

MyExcel::~MyExcel() {
    while(excel->Workbooks()->Count())
        excel->ActiveWorkbook()->Close(false);
    excel->Quit();
}

void MyExcel::loadFile(const QString& fileName, MyTableModel* m_model) {
    excel->Workbooks()->Open(fileName);
    //    QAxObject* excel = new QAxObject("Excel.Application", 0);
    //    excel->dynamicCall("SetVisible(bool)", true);

    //    QAxObject* workbooks = excel->querySubObject("Workbooks");
    //    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileName);
    //    QAxObject* sheets = workbook->querySubObject("Worksheets");

    if(excel->Workbooks()->Count()) {

        //auto values= excel->Range("");

        //        for(int devCh = 0; devCh < RowCount; ++devCh) {
        //            m_model->clearData(devCh);
        //            for(int adcCh = 0; adcCh < 2; ++adcCh) {
        //                for(int resCh = 0; resCh < 3; ++resCh) {
        //                    QVariant value;
        //                    if(!adcCh) {
        //                        QAxObject* cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 4);
        //                        value = cell->dynamicCall("Value()");
        //                    } else {
        //                        QAxObject* cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 7);
        //                        value = cell->dynamicCall("Value()");
        //                    }
        //                    m_model->addData(devCh, adcCh * 3 + resCh, value.toDouble());
        //                }
        //            }
        //        }
        excel->ActiveWorkbook()->Close(false);
    }
}

void MyExcel::saveFile(const QString& fileName, const QString& asptNum, const QString& fio, MyTableModel* m_model) {
    excel->Workbooks()->Open(fileName);

    //    const bool newFile = !QFileInfo::exists(fileName);

    //    QAxObject* excel = new QAxObject("Excel.Application", 0);
    //    excel->dynamicCall("SetVisible(bool)", true);

    //    QAxObject* workbooks = excel->querySubObject("Workbooks");
    //    QAxObject* workbook = nullptr;

    //    if(newFile)
    //        workbook = workbooks->querySubObject("Open(const QString&)", qApp->applicationDirPath() + "/blank.xlsx");
    //    else
    //        workbook = workbooks->querySubObject("Open(const QString&)", fileName);

    //    QAxObject* sheets = workbook->querySubObject("Worksheets");
    //    if(workbooks->dynamicCall("Count()").toInt()) {
    //        QAxObject* sheet = sheets->querySubObject("Item(int)", 1);
    //        QAxObject* cell;
    //        if(newFile) {
    //            cell = sheet->querySubObject("Cells(int,int)", 55, 4);
    //            cell->dynamicCall("SetValue2(QVariant)", QDateTime::currentDateTime());
    //        }
    //        cell = sheet->querySubObject("Cells(int,int)", 2, 5);
    //        cell->dynamicCall("SetValue2(QVariant)", asptNum);
    //        cell = sheet->querySubObject("Cells(int,int)", 57, 4);
    //        cell->dynamicCall("SetValue2(QVariant)", fio);
    //        cell = sheet->querySubObject("Cells(int,int)", 56, 4);
    //        cell->dynamicCall("SetValue2(QVariant)", QDateTime::currentDateTime());
    //        for(int devCh = 0; devCh < RowCount; ++devCh) {
    //            for(int adcCh = 0; adcCh < 2; ++adcCh) {
    //                for(int resCh = 0; resCh < 3; ++resCh) {
    //                    QAxObject* cell;
    //                    if(!adcCh) {
    //                        cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 4);
    //                    } else {
    //                        cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 7);
    //                    }
    //                    cell->dynamicCall("SetValue2(QVariant)", m_model->getData(devCh).average(adcCh * 3 + resCh));
    //                }
    //            }
    //        }

    //        if(newFile)
    //            workbook->dynamicCall("SaveAs(const QVariant&)", QVariant(QString(fileName).replace('/', '\\')));
    //        else
    //            workbook->dynamicCall("Save()");

    //        workbook->dynamicCall("Close()");
    //    }
    //    excel->dynamicCall("Quit()");
    //    excel->deleteLater();
}

void MyExcel::printFile(const QString& fileName) {
    //    QAxObject* excel = new QAxObject("Excel.Application", 0);
    //    excel->dynamicCall("SetVisible(bool)", true);
    //    QAxObject* workbooks = excel->querySubObject("Workbooks");
    //    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", fileName);
    //    QAxObject* sheets = workbook->querySubObject("Worksheets");
    //    if(workbooks->dynamicCall("Count()").toInt()) {
    //        QAxObject* sheet = sheets->querySubObject("Item(int)", 1);
    //        sheet->dynamicCall("PrintOut(QVariant, QVariant, QVariant)", 1, 1, 1);
    //        workbook->dynamicCall("Close()");
    //    }
    //    excel->dynamicCall("Quit()");
    //    excel->deleteLater();
}
