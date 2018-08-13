#include "table.h"
#include "myheader.h"
#include "mytablemodel.h"
#include <QApplication>
#include <QAxObject>
#include <QDateTime>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QTimer>

Table::Table(QWidget* parent)
    : QTableView(/*RowCount, ColumnCount, */ parent)
    , m_model(new MyTableModel(this))
#ifdef EXCEL
    , excel(new Excel::Application(this))
#endif
{
    setModel(m_model);

    QAbstractButton* cornerButton = findChild<QAbstractButton*>();
    if (cornerButton) {
        QCheckBox* c = new QCheckBox("№", cornerButton);
        c->setGeometry(cornerButton->rect() + QMargins(-5, 0, 100, 0));
        connect(c, &QCheckBox::toggled, [this](bool checked) { reinterpret_cast<MyHeader*>(verticalHeader())->setChecked(checked); });
    }

    //    setHorizontalHeader(new MyHeader(Qt::Horizontal, this));
    setVerticalHeader(new MyHeader(Qt::Vertical, this));

    connect(m_model, &MyTableModel::dataChanged, [=](const QModelIndex& topLeft, const QModelIndex& /*bottomRight*/, const QVector<int>& /*roles*/) {
        updatePlot(topLeft.row());
    });

    connect(reinterpret_cast<MyHeader*>(verticalHeader()), &MyHeader::checkedChanged, m_model, &MyTableModel::setRowsEnabled);

    setColumnHidden(MeasureDeltaCh0, true);
    //    setColumnHidden(MeasurePpmCh0, true);
    setColumnHidden(MeasureDeltaCh1, true);
    //    setColumnHidden(MeasurePpmCh1, true);

    setIconSize(QSize(24, 24));

    horizontalHeader()->setSectionResizeMode(SignalCh0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(MeasureDeltaCh0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(MeasureCh0, QHeaderView::Stretch);

    horizontalHeader()->setSectionResizeMode(SignalCh1, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(MeasureDeltaCh1, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(MeasureCh1, QHeaderView::Stretch);

    //        Interactive,
    //        Stretch,
    //        Fixed,

    //    resizeEvent(nullptr);

#ifdef EXCEL
    excel->SetVisible(true);
#endif
}

Table::~Table()
{
#ifdef EXCEL
    excel->Quit();
#endif
}

void Table::loadFile(const QString& fileName)
{
    qDebug() << "LoadFile" << fileName;
#ifdef EXCEL
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
#else
    m_curFile = fileName;
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible(bool)", true);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", m_curFile);
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
        resizeEvent(0);
    }
    excel->dynamicCall("Quit()");
    excel->deleteLater();
#endif
}

void Table::saveFile(const QString& fileName, const QString& asptNum, const QString& fio)
{
    qDebug() << "SaveFile" << fileName << asptNum << fio;
#ifdef EXCEL
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
#else
    bool newFile = false;
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    excel->dynamicCall("SetVisible(bool)", true);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    if (m_curFile.isEmpty()) {
        m_curFile = qApp->applicationDirPath() + "/blank.xlsx";
        newFile = true;
    }
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", m_curFile);
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
        if (newFile) {
            workbook->dynamicCall("SaveAs(const QVariant&)", QVariant(QString(fileName).replace('/', '\\')));
        } else {
            workbook->dynamicCall("Save()");
        }
        workbook->dynamicCall("Close()");
    }
    excel->dynamicCall("Quit()");
    excel->deleteLater();
#endif
}

void Table::printFile(const QString& fileName)
{
    qDebug() << "PrintFile" << fileName;
#ifdef EXCEL
    if (!excel->isNull()) {
        if (excel->Workbooks()->Open(fileName)) {
            Excel::_Worksheet sheet(excel->ActiveSheet());
            sheet.PrintOut(1, 1, 1);
            excel->Workbooks()->Close(); //ActiveWorkbook()->Close(false);
        }
    }
#else
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
#endif
}

MyTableModel* Table::model() const { return m_model; }

void Table::enableDelta(bool checked)
{
    setColumnHidden(MeasureDeltaCh0, !checked);
    setColumnHidden(MeasureDeltaCh1, !checked);
}

QVector<bool> Table::checkedRows() const
{
    return reinterpret_cast<MyHeader*>(verticalHeader())->m_checked;
}

void Table::setCurrentFile(const QString& value)
{
    m_curFile = value;
}
