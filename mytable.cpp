#include "mytable.h"
#include "myheader.h"
#include "mytablemodel.h"
#include <QApplication>
#include <QAxObject>
#include <QDateTime>
#include <QFileInfo>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QTimer>

MyTable::MyTable(QWidget* parent)
    : QTableView(/*RowCount, ColumnCount, */ parent)
    , m_header(new MyHeader(Qt::Vertical, this))
    , m_model(new MyTableModel(this))
{
    setModel(m_model);
    setVerticalHeader(m_header);

    connect(m_model, &MyTableModel::dataChanged, [=](const QModelIndex& topLeft, const QModelIndex& /*bottomRight*/, const QVector<int>& /*roles*/) {
        updatePlot(topLeft.row());
        resizeRowToContents(topLeft.row());
    });

    QAbstractButton* cornerButton = findChild<QAbstractButton*>();
    if (cornerButton) {
        c = new QCheckBox("№", cornerButton);
        c->setGeometry(cornerButton->rect() + QMargins(-5, 0, 100, 0));
        connect(c, &QCheckBox::toggled, [this](bool checked) { m_header->setChecked(checked); });
    }

    connect(m_header, &MyHeader::checkedChanged, m_model, &MyTableModel::setRowsEnabled);

    setColumnHidden(MeasureDeltaCh0, true);
    setColumnHidden(MeasureDeltaCh1, true);

    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(MeasureCh0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(MeasureCh1, QHeaderView::Stretch);

    setIconSize(QSize(24, 24));
}

MyTable::~MyTable()
{
}

void MyTable::loadFile(const QString& fileName)
{
    qDebug() << "LoadFile" << fileName;

    QFile file(QString("%1/proto/%2.bin").arg(qApp->applicationDirPath()).arg(QFileInfo(fileName).fileName()));
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        for (int i = 0; i < RowCount; ++i) {
            m_model->clearData(i);
            in >> m_model->getData(i);
        }
        qDebug() << file;
        file.close();
        resizeRowsToContents();
        return;
    }
    ExcelAx::loadFile(fileName, m_model);
    resizeRowsToContents();
}

void MyTable::saveFile(const QString& fileName, const QString& asptNum, const QString& fio)
{
    qDebug() << "SaveFile" << fileName << asptNum << fio;
    saveFile(fileName);
    ExcelAx::saveFile(fileName, asptNum, fio, m_model);
}

void MyTable::saveFile(const QString& fileName)
{
    QFile file(QString("%1/proto/%2.bin").arg(qApp->applicationDirPath()).arg(QFileInfo(fileName).fileName()));
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for (int i = 0; i < RowCount; ++i)
            out << m_model->getData(i);
        file.close();
    }
}

void MyTable::printFile(const QString& fileName)
{
    qDebug() << "PrintFile" << fileName;

    ExcelAx::printFile(fileName);
}

MyTableModel* MyTable::model() const { return m_model; }

void MyTable::enableDelta(bool checked)
{
    setColumnHidden(MeasureDeltaCh0, !checked);
    setColumnHidden(MeasureDeltaCh1, !checked);
}

QVector<bool> MyTable::checkedRows() const { return m_header->m_checked; }

void MyTable::setEnabledCheckBoxes(bool enabled)
{
    verticalHeader()->setEnabled(enabled);
    if (c)
        c->setEnabled(enabled);
}
