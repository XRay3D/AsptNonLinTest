#include "excelex.h"
#include "mytablemodel.h"
#include <QApplication>
#include <QAxObject>
#include <QDateTime>

ExcelEx::ExcelEx(QObject* parent)
    : excel(new Excel::Application(this))
{
    excel->SetVisible(true);
}

ExcelEx::~ExcelEx()
{
    excel->Quit();
}

void ExcelEx::loadFile(const QString& fileName, MyTableModel* m_model)
{
    if (!excel->isNull()) {
        Excel::_Workbook pWb(0, excel->Workbooks()->querySubObject("Open(const QString&)", fileName) /*->Open(fileName)*/);
        if (excel->Workbooks()->Count()) {
            for (int devCh = 0; devCh < RowCount; ++devCh) {
                for (int adcCh = 0; adcCh < 2; ++adcCh) {
                    for (int resCh = 0; resCh < 3; ++resCh) {
                        data[devCh][adcCh * 3 + resCh].clear();
                        if (!adcCh)
                            data[devCh][adcCh * 3 + resCh].append(excel->Range(QString("D%1").arg(6 + devCh * 3 + resCh))->Value().toDouble());
                        else
                            data[devCh][adcCh * 3 + resCh].append(excel->Range(QString("G%1").arg(6 + devCh * 3 + resCh))->Value().toDouble());
                    }
                }
                Update(devCh);
            }
            pWb.Close();
            //excel->Workbooks()->Close();
            dataChanged = true;
            resizeEvent(0);
        }
    }
}

void ExcelEx::saveFile(QString& fileName, const QString& asptNum, const QString& fio, MyTableModel* m_model)
{
    if (!excel->isNull()) {
        if (curFile.isEmpty()) {
            excel->Workbooks()->Open(qApp->applicationDirPath() + "/XX-XX от XX.XX.XX г.xlsx");
            excel->Range("D55")->SetValue(QDateTime::currentDateTime());
        } else {
            excel->Workbooks()->Open(curFile);
        }
        excel->Range("E2")->SetValue(asptNum);
        excel->Range("D57")->SetValue(fio);
        excel->Range("D56")->SetValue(QDateTime::currentDateTime());
        for (int devCh = 0; devCh < RowCount; ++devCh) {
            for (int adcCh = 0; adcCh < 2; ++adcCh) {
                for (int resCh = 0; resCh < 3; ++resCh) {
                    int num;
                    double average;
                    num = devCh * 6 + adcCh * 3 + resCh;
                    average = 0.0;
                    foreach (double val, data[num]) {
                        average += val;
                    }
                    average /= data[num].count();
                    if (!adcCh) {
                        excel->Range(QString("D%1").arg(6 + devCh * 3 + resCh))->SetValue(average);
                    } else {
                        excel->Range(QString("G%1").arg(6 + devCh * 3 + resCh))->SetValue(average);
                    }
                }
            }
        }
        QString str = fileName;
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-1251"));
        if (curFile.isEmpty() || QString(*curFile) != fileName) {
            //            qDebug("if");
            excel->ActiveWorkbook()->SaveAs(str.replace('/', '\\'));
            //excel->ActiveWorkbook()->SaveAs(str.replace('/', '\\'),Excel::xlOpenXMLWorkbook,"","",);        }
        } else {
            //            qDebug("else");
            excel->ActiveWorkbook()->Save();
        }
        while (excel->Workbooks()->Count() > 0) {
            //            qDebug() << excel->ActiveWorkbook()->Name();
            excel->ActiveWorkbook()->Close(false);
        }
    }
}

void ExcelEx::printFile(const QString& fileName)
{
    if (!excel->isNull()) {
        if (excel->Workbooks()->Open(fileName)) {
            Excel::_Worksheet sheet(excel->ActiveSheet());
            sheet.PrintOut(1, 1, 1);
            excel->Workbooks()->Close(); //ActiveWorkbook()->Close(false);
        }
    }
}
