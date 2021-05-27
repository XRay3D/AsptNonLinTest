#include "mytable.h"

#if __has_include("myexcel.h")
#include "myexcel.h"
#define EXCEL 1
#endif

#include "measuremodel.h"
#include "myheader.h"
#include <QApplication>
#include <QAxObject>
#include <QCheckBox>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QScrollBar>
#include <QSettings>
#include <QTableView>
#include <QTableWidgetItem>
#include <QTimer>
#include <QWidget>

MyTable::MyTable(QWidget* parent)
    : QTableView(/*RowCount, ColumnCount, */ parent)
    , m_header(new MyHeader(Qt::Vertical, this))
    , m_model(new MeasureModel(this))
{
    setModel(m_model);
    setVerticalHeader(m_header);

    connect(m_model, &MeasureModel::dataChanged, [=](const QModelIndex& topLeft, const QModelIndex& /*bottomRight*/, const QVector<int>& /*roles*/) {
        updatePlot(topLeft.row());
        resizeRowToContents(topLeft.row());
    });

    QAbstractButton* cornerButton = findChild<QAbstractButton*>();
    if (cornerButton) {
        cornerCheckBox = new QCheckBox("№", cornerButton);
        cornerCheckBox->setGeometry(cornerButton->rect() + QMargins(-5, 0, 100, 0));
        connect(cornerCheckBox, &QCheckBox::clicked, [this] {
            if (cornerCheckBox->checkState() == Qt::PartiallyChecked)
                cornerCheckBox->setCheckState(Qt::Checked);
            m_header->setChecked(cornerCheckBox->checkState());
        });
    }

    connect(m_header, &MyHeader::checkedChanged, m_model, &MeasureModel::setRowsEnabled);
    connect(m_header, &MyHeader::checkedChanged, m_model, [this](const std::vector<Qt::CheckState>& checkStates, Qt::Orientation) {
        constexpr Qt::CheckState chState[] {
            Qt::Unchecked,
            Qt::Unchecked,
            Qt::Checked,
            Qt::PartiallyChecked
        };
        int fl {};
        for (auto checkState : checkStates) {
            fl |= checkState == Qt::Checked ? 2 : 0;
            fl |= checkState == Qt::Unchecked ? 1 : 0;
            if (fl == 3)
                break;
        }
        cornerCheckBox->setCheckState(chState[fl]);
    });

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

void MyTable::openFile(const QString& fileName)
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
#ifdef EXCEL
    MyExcel().loadFile(fileName, m_model);
#endif
    resizeRowsToContents();
}

void MyTable::saveFile(const QString& fileName, const QString& asptNum, const QString& fio)
{
    qDebug() << "SaveFile" << fileName << asptNum << fio;
    saveFile(fileName);
#ifdef EXCEL
    MyExcel().saveFile(fileName, asptNum, fio, m_model);
#endif
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
#ifdef EXCEL
    MyExcel().printFile(fileName);
#endif
}

MeasureModel* MyTable::model() const { return m_model; }

void MyTable::enableDelta(bool checked)
{
    setColumnHidden(MeasureDeltaCh0, !checked);
    setColumnHidden(MeasureDeltaCh1, !checked);
}

std::vector<Qt::CheckState> MyTable::checkedRows() const { return m_header->m_checked; }

void MyTable::setEnabledCheckBoxes(bool enabled)
{
    verticalHeader()->setEnabled(enabled);
    if (cornerCheckBox)
        cornerCheckBox->setEnabled(enabled);
}
