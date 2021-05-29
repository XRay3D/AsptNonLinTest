#include "mytable.h"

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
    , m_header(new MyHeader(Qt::Vertical, this)) //
{
    setVerticalHeader(m_header);

    setIconSize(QSize(24, 24));
}

MyTable::~MyTable() { }

void MyTable::enableDelta(bool checked) {
    setColumnHidden(MeasureDeltaCh0, !checked);
    setColumnHidden(MeasureDeltaCh1, !checked);
}

std::vector<Qt::CheckState> MyTable::checkedRows() const { return m_header->m_checked; }

void MyTable::setEnabledCheckBoxes(bool enabled) {
    verticalHeader()->setEnabled(enabled);
    if(cornerCheckBox)
        cornerCheckBox->setEnabled(enabled);
}

void MyTable::setModel(QAbstractItemModel* model) {
    QTableView::setModel(model);
    QAbstractButton* cornerButton = findChild<QAbstractButton*>();
    if(cornerButton) {
        cornerCheckBox = new QCheckBox("№", cornerButton);
        cornerCheckBox->setGeometry(cornerButton->rect() + QMargins(-5, 0, 100, 0));
        connect(cornerCheckBox, &QCheckBox::clicked, [this] {
            if(cornerCheckBox->checkState() == Qt::PartiallyChecked)
                cornerCheckBox->setCheckState(Qt::Checked);
            m_header->setChecked(cornerCheckBox->checkState());
        });
    }

    setColumnHidden(MeasureDeltaCh0, true);
    setColumnHidden(MeasureDeltaCh1, true);

    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(MeasureCh0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(MeasureCh1, QHeaderView::Stretch);
    connect(m_header, &MyHeader::checkedChanged, static_cast<MeasureModel*>(model), &MeasureModel::setRowsEnabled);
    connect(m_header, &MyHeader::checkedChanged, static_cast<MeasureModel*>(model), [this](const std::vector<Qt::CheckState>& checkStates, Qt::Orientation) {
        constexpr Qt::CheckState chState[]{
            Qt::Unchecked,
            Qt::Unchecked,
            Qt::Checked,
            Qt::PartiallyChecked};
        int fl{};
        for(auto checkState : checkStates) {
            fl |= checkState == Qt::Checked ? 2 : 0;
            fl |= checkState == Qt::Unchecked ? 1 : 0;
            if(fl == 3)
                break;
        }
        cornerCheckBox->setCheckState(chState[fl]);
    });
}
