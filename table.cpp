#include "table.h"
#include "mytablemodel.h"
#include <QApplication>
#include <QAxObject>
#include <QDateTime>
#include <QPainter>
#include <QSettings>
#include <QTimer>

enum ColumnType {
    //    RowNum,
    SignalCh0,
    MeasureDeltaCh0,
    MeasurePpmCh0,
    MeasureCh0,
    SignalCh1,
    MeasureDeltaCh1,
    MeasurePpmCh1,
    MeasureCh1
};

enum {
    RowCount = 16,
    ColumnCount = 9
};

Table::Table(QWidget* parent)
    : QTableView(/*RowCount, ColumnCount, */ parent)
    , m_dataChanged(false)
    , m_resistors({ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 })
    , m_average(RowCount)
    , m_data(RowCount)
    , m_cellText(RowCount)
#ifdef EXCEL
    , excel(new Excel::Application(this))
#endif
{
    setModel(new MyTableModel(this));

    //    setHorizontalHeader(new MyHeader(Qt::Horizontal, this));
    setVerticalHeader(new MyHeader(Qt::Vertical, this));

    //    connect(reinterpret_cast<MyHeader*>(horizontalHeader()), &MyHeader::stateChanged, m_model, &MyModel::stateChanged);
    //    connect(reinterpret_cast<MyHeader*>(horizontalHeader()), &MyHeader::stateChanged, [&](const QMap<int, bool>& enabled, int /*orientation*/) {
    //        for (int i = 0; i < enabled.size(); ++i)
    //            if (enabled[i])
    //                m_currentColumn = i;
    //    });
    //    connect(reinterpret_cast<MyHeader*>(verticalHeader()), &MyHeader::stateChanged, m_model, &MyModel::stateChanged);

    //    for (QVector<QVector<double>>& v : m_data) {
    //        v.resize(6);
    //        for (QVector<double>& d : v) {
    //            d.fill(0.0, 1);
    //        }
    //    }

    //    for (QVector<double>& v : m_average) {
    //        v.fill(0.0, 6);
    //    }

    //    for (QVector<QVector<QString>>& v : m_cellText) {
    //        v.resize(5);
    //        int i = 0;
    //        for (QVector<QString>& d : v) {
    //            switch (i++) {
    //            case RowNum:
    //                break;
    //            case SignalCh0:
    //            case MeasureDeltaCh0:
    //            case MeasurePpmCh0:
    //                d.fill("", 6);
    //                break;
    //            case MeasureCh0:
    //                d.fill("", 2);
    //                break;
    //            }
    //        };
    //    }

    //    QFont f(font());
    //    f.setPointSizeF(f.pointSizeF() * 3);

    //    setHorizontalHeaderLabels({ "№", "Сигнал, Ом", "∆, мОм", "PPM", "Канал АЦП 0", "Сигнал, Ом", "∆, мОм", "PPM", "Канал АЦП 1" });
    //    verticalHeader()->setVisible(false);

    //    for (int row = 0; row < rowCount(); ++row) {
    //        for (int col = 0; col < columnCount(); ++col) {
    //            QTableWidgetItem* tableitem = new QTableWidgetItem("QTableWidgetItem");
    //            setItem(row, col, tableitem);

    //            switch (col) {
    //            case RowNum:
    //                tableitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    //                tableitem->setCheckState(Qt::Checked);
    //                tableitem->setTextAlignment(Qt::AlignCenter);
    //                tableitem->setText(QString("%1").arg(row + 1));
    //                break;
    //            case SignalCh0:
    //            case MeasureDeltaCh0:
    //            case MeasurePpmCh0:
    //            case SignalCh1:
    //            case MeasureDeltaCh1:
    //            case MeasurePpmCh1:
    //                tableitem->setFlags(Qt::ItemIsEnabled);
    //                break;
    //            case MeasureCh0:
    //            case MeasureCh1:
    //                tableitem->setFlags(Qt::ItemIsEnabled);
    //                tableitem->setFont(f);
    //                break;
    //            }
    //        }
    //        updateRow(row);
    //    }
    //    m_dataChanged = true;
    //    resizeEvent(0);

    setColumnHidden(MeasureDeltaCh0, true);
    setColumnHidden(MeasurePpmCh0, true);
    setColumnHidden(MeasureDeltaCh1, true);
    setColumnHidden(MeasurePpmCh1, true);

    //    connect(this, &QTableWidget::itemClicked, [&](QTableWidgetItem* Item) {
    //        if (Item->column() == RowNum) {
    //            for (int col = SignalCh0; col < columnCount(); ++col) {
    //                item(Item->row(), col)->setFlags(Item->checkState() == Qt::Checked ? Qt::ItemIsEnabled : Qt::NoItemFlags);
    //            }
    //        }
    //    });

    //    setIconSize(QSize(24, 24));

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
            for (int adcCh = 0; adcCh < 2; ++adcCh) {
                for (int resCh = 0; resCh < 3; ++resCh) {
                    m_data[devCh][adcCh * 3 + resCh].clear();
                    if (!adcCh) {
                        QAxObject* cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 4);
                        value = cell->dynamicCall("Value()");
                    } else {
                        QAxObject* cell = sheet->querySubObject("Cells(int,int)", 6 + devCh * 3 + resCh, 7);
                        value = cell->dynamicCall("Value()");
                    }
                    m_data[devCh][adcCh * 3 + resCh].append(value.toDouble());
                }
            }
            updateRow(devCh);
        }
        workbook->dynamicCall("Close()");
        m_dataChanged = true;
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
                    cell->dynamicCall("SetValue2(QVariant)", m_average[devCh][adcCh * 3 + resCh]);
                }
            }
            updateRow(devCh);
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

void Table::updateRow(int row)
{
    for (int i = 0; i < 6; ++i)
        updateRow(row, i);
}

void Table::updateRow(int row, int pos)
{
    //    double delta;
    //    double min_ = 0.0;
    //    double max_ = 0.0;

    //    if (m_data[row][pos].count()) {
    //        QVector<double> v(m_data[row][pos]);
    //        qSort(v);
    //        min_ = v.first();
    //        max_ = v.last();
    //        m_average[row][pos] = 0;
    //        if (v.count() > m_skip) {
    //            for (int i = static_cast<int>(ceil(m_skip * 0.5)); i < (m_data[row][pos].count() - floor(m_skip * 0.5)); ++i) {
    //                m_average[row][pos] += v[i];
    //            }
    //            m_average[row][pos] /= v.count() - m_skip;
    //        } else {
    //            for (double val : v) {
    //                m_average[row][pos] += val;
    //            }
    //            m_average[row][pos] /= v.count();
    //        }
    //    }

    //    int i = (pos % 3);

    //    m_cellText[row][SignalCh0][pos] = QString("R%1 = %2%3")
    //                                          .arg(i + 1)
    //                                          .arg(m_average[row][pos], 0, 'f', 5)
    //                                          .arg(i < 2 ? "\n" : "")
    //                                          .replace('.', ',');

    //    m_cellText[row][MeasureDeltaCh0][pos] = QString("%1%2")
    //                                                .arg((max_ - min_) * 1000.0, 0, 'g', 3)
    //                                                .arg(i < 2 ? "\n" : "")
    //                                                .replace('.', ',');

    //    m_cellText[row][MeasurePpmCh0][pos] = QString("%1%2")
    //                                              .arg(m_resistors[i] > 0.0 ? ((m_average[row][pos] / m_resistors[i]) - 1.0) * 1.e6 : 0.0, 0, 'g', 4)
    //                                              .arg(i < 2 ? "\n" : "");

    //    QColor color;
    //    if (pos < 3) {
    //        delta = m_average[row][0] + m_average[row][1] - m_average[row][2];
    //        m_cellText[row][MeasureCh0][0] = QString("%1%2").arg(delta < 0 ? "" : "").arg(delta, 0, 'f', 6).replace('.', ',');
    //    } else {
    //        delta = m_average[row][3] + m_average[row][4] - m_average[row][5];
    //        m_cellText[row][MeasureCh0][1] = QString("%1%2").arg(delta < 0 ? "" : "").arg(delta, 0, 'f', 6).replace('.', ',');
    //    }

    //    delta = abs(delta);
    //    QIcon icon;

    //    if (delta > m_max || delta == 0.0) {
    //        color = QColor::fromHsv(0, 50, 255);
    //        icon = QIcon("icon1.svg");
    //    } else if (delta < m_min) {
    //        color = QColor::fromHsv(120, 50, 255);
    //        icon = QIcon("icon2.svg");
    //    } else {
    //        color = QColor::fromHsv(120 - static_cast<int>((120 / (m_max - m_min)) * (delta - m_min)), 50, 255);
    //        icon = QIcon("icon3.svg");
    //    }

    //    if (pos < 3) {
    //        item(row, SignalCh0)->setText(m_cellText[row][SignalCh0][0] + m_cellText[row][SignalCh0][1] + m_cellText[row][SignalCh0][2]);
    //        item(row, MeasureDeltaCh0)->setText(m_cellText[row][MeasureDeltaCh0][0] + m_cellText[row][MeasureDeltaCh0][1] + m_cellText[row][MeasureDeltaCh0][2]);
    //        item(row, MeasurePpmCh0)->setText(m_cellText[row][MeasurePpmCh0][0] + m_cellText[row][MeasurePpmCh0][1] + m_cellText[row][MeasurePpmCh0][2]);
    //        item(row, MeasureCh0)->setText(m_cellText[row][MeasureCh0][0]);
    //        item(row, MeasureCh0)->setBackgroundColor(color);
    //        //item(row, MeasureCh0)->setForeground(Qt::black);
    //        item(row, MeasureCh0)->setIcon(icon);
    //    } else {
    //        item(row, SignalCh1)->setText(m_cellText[row][SignalCh0][3] + m_cellText[row][SignalCh0][4] + m_cellText[row][SignalCh0][5]);
    //        item(row, MeasureDeltaCh1)->setText(m_cellText[row][MeasureDeltaCh0][3] + m_cellText[row][MeasureDeltaCh0][4] + m_cellText[row][MeasureDeltaCh0][5]);
    //        item(row, MeasurePpmCh1)->setText(m_cellText[row][MeasurePpmCh0][3] + m_cellText[row][MeasurePpmCh0][4] + m_cellText[row][MeasurePpmCh0][5]);
    //        item(row, MeasureCh1)->setText(m_cellText[row][MeasureCh0][1]);
    //        item(row, MeasureCh1)->setBackgroundColor(color);
    //        //item(row, MeasureCh1)->setForeground(Qt::black);
    //        item(row, MeasureCh1)->setIcon(icon);
    //    }
}

void Table::checkUncheckAll(bool checked)
{
    //    for (int row = 0; row < RowCount; ++row) {
    //        item(row, 0)->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    //        for (int col = 1; col < columnCount(); ++col) {
    //            item(row, col)->setFlags(checked ? Qt::ItemIsEnabled : Qt::NoItemFlags);
    //        }
    //    }
}

void Table::enablePpm(bool checked)
{
    setColumnHidden(MeasurePpmCh0, !checked);
    setColumnHidden(MeasurePpmCh1, !checked);
    m_dataChanged = true;
    resizeEvent(0);
}

void Table::enableDelta(bool checked)
{
    setColumnHidden(MeasureDeltaCh0, !checked);
    setColumnHidden(MeasureDeltaCh1, !checked);
    m_dataChanged = true;
    resizeEvent(0);
}

void Table::clearSelectedData()
{
    //    QList<int> rows;
    //    for (int row = 0; row < RowCount; ++row) {
    //        if (item(row, 0)->checkState() == Qt::Checked) {
    //            rows.append(row);
    //        }
    //    }
    //    if (rows.size()) {
    //        if (QMessageBox::question(this, "", "Вы действительно хотите отчистить выделенные каналы?", "Да", "Нет", "", 1, 1) == 0) {
    //            for (int row : rows) {
    //                for (QVector<double>& v : m_data[row]) {
    //                    v.fill(0.0, 1);
    //                }
    //                updateRow(row);
    //            }
    //            m_dataChanged = true;
    //            resizeEvent(0);
    //        }
    //    }
}

void Table::clearData(int row)
{
    //    if (0 > row || row > 15) {
    //        return;
    //    }
    //    for (QVector<double>& v : m_data[row]) {
    //        v.clear();
    //    }
}

void Table::setResistorsValue(const QVector<double>&& value)
{
    //    m_resistors = value;
}

void Table::resizeEvent(QResizeEvent* event)
{
    if (event != nullptr) {
        event->accept();
    }
    //    static QSize Size;
    //    if (Size != size() || dataChanged) {
    if (m_dataChanged) {
        resizeColumnsToContents();
        resizeRowsToContents();
    }
    int ColumnWidth = (size().width() - 2) - (/*columnWidth(RowNum) +*/ verticalHeader()->width() + columnWidth(SignalCh0) + columnWidth(SignalCh1));
    if (!isColumnHidden(MeasureDeltaCh0)) {
        ColumnWidth -= (columnWidth(MeasureDeltaCh0) + columnWidth(MeasureDeltaCh1));
    }
    if (!isColumnHidden(MeasurePpmCh0)) {
        ColumnWidth -= (columnWidth(MeasurePpmCh0) + columnWidth(MeasurePpmCh1));
    }
    if (verticalScrollBar()->isVisible()) {
        ColumnWidth -= verticalScrollBar()->width();
    }
    setColumnWidth(MeasureCh0, static_cast<int>(ColumnWidth / 2.0));
    setColumnWidth(MeasureCh1, static_cast<int>(ColumnWidth / 2.0));
    m_dataChanged = false;
    //    Size = size();
    //    }
    update();
}

void Table::setCurrentFile(const QString& value)
{
    m_curFile = value;
}

void Table::enableRow(bool checked)
{
    //    if (checked) {
    //        for (int row = 0; row < RowCount; ++row) {
    //            item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    //        }
    //    } else {
    //        for (int row = 0; row < RowCount; ++row) {
    //            item(row, 0)->setFlags(Qt::ItemIsEnabled);
    //        }
    //    }
}

QVector<double> Table::getData(int row, int pos) const
{
    return m_data[row][pos];
}

void Table::addData(int row, int pos, double value)
{
    m_data[row][pos].append(value);
    updateRow(row, pos);
    m_dataChanged = true;
    resizeEvent(0);
    emit updatePlot(row);
}

void Table::setSkip(int value)
{
    m_skip = value;
}

void Table::setMax(double value)
{
    m_max = value;
    m_dataChanged = true;
}

void Table::setMin(double value)
{
    m_min = value;
    m_dataChanged = true;
}

void Table::showEvent(QShowEvent* event)
{
    //    event->accept();
    //    if (m_dataChanged) {
    //        for (int row = 0; row < rowCount(); ++row) {
    //            updateRow(row);
    //        }
    //    }
    resizeEvent(0);
}

//////////////////////////////////////////////////////////////
/// \brief MyHeader::MyHeader
/// \param orientation
/// \param parent
///
MyHeader::MyHeader(Qt::Orientation orientation, QWidget* parent)
    : QHeaderView(orientation, parent)
    , m_isOn(16, false)
{
    QSettings settings;
    int size = settings.beginReadArray("MyHeader" + QString::number(orientation));
    for (int i = 0, key = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        key = settings.value("key").toInt();
        if (key > -1)
            m_isOn[key] = settings.value("value").toBool();
    }
    settings.endArray();
    //Отправить сигнал после установки всех соединений.
    QTimer::singleShot(1, Qt::CoarseTimer, [=] { emit stateChanged(m_isOn, orientation); });
}

MyHeader::~MyHeader()
{
    QSettings settings;
    settings.beginWriteArray("MyHeader" + QString::number(orientation()));
    for (int i = 0; i < m_isOn.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("key", i);
        settings.setValue("value", m_isOn[i]);
    }

    settings.endArray();
}

void MyHeader::mouseMoveEvent(QMouseEvent* event)
{
    if (orientation() == Qt::Horizontal)
        return;
    static int index = 0;
    if (index != logicalIndexAt(event->pos())) {
        index = logicalIndexAt(event->pos());
        if (index < 0)
            return;
        if (event->buttons() & Qt::LeftButton) {
            qDebug() << "mouseMoveEvent" << event;
            m_isOn[index] = !m_isOn[index];
            updateSection(index);
            stateChanged(m_isOn, orientation());
        }
    }
}

void MyHeader::mousePressEvent(QMouseEvent* event)
{
    int index = logicalIndexAt(event->pos());
    if (index < 0)
        return;
    if (orientation() == Qt::Horizontal) {
        for (int var = 0; var < count(); ++var)
            m_isOn[var] = false;
        m_isOn[index] = true;
        for (int var = 0; var < count(); ++var)
            updateSection(var);
    } else {
        m_isOn[index] = !m_isOn[index];
        updateSection(index);
    }

    stateChanged(m_isOn, orientation());

    QHeaderView::mousePressEvent(event);
}

void MyHeader::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    painter->save();
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->restore();

    QStyleOptionButton option;
    option.rect = QRect(rect.left() + 4, (rect.height() - 16) / 2 + rect.top(), 16, 16);

    m_isOn[logicalIndex] ? option.state = QStyle::State_On : option.state = QStyle::State_Off;
    option.state |= (isEnabled() ? QStyle::State_Enabled : QStyle::State_None);
    if (orientation() == Qt::Horizontal)
        style()->drawPrimitive(QStyle::PE_IndicatorRadioButton, &option, painter);
    else
        style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &option, painter);
}

QSize MyHeader::sizeHint() const
{
    return QSize(40, 40);
}
