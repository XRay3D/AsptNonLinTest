#include "mytablemodel.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

MyTableModel::MyTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_rowEnabled{false} {
    restore();
}

MyTableModel::~MyTableModel() {
    save();
}

void MyTableModel::setRowsEnabled(const QVector<bool>& rowEnabled, int) {
    for(int i = 0; i < RowCount; ++i) {
        if(m_rowEnabled[i] != rowEnabled[i]) {
            m_rowEnabled[i] = rowEnabled[i];
            emit dataChanged(index(i, SignalCh0), index(i, MeasureCh1), {Qt::DisplayRole});
        }
    }
}

void MyTableModel::setMax(double value) {
    RowData::m_max = value;
    emit dataChanged(index(0, SignalCh0), index(15, MeasureCh1), {Qt::DisplayRole});
}

void MyTableModel::setMin(double value) {
    RowData::m_min = value;
    emit dataChanged(index(0, SignalCh0), index(15, MeasureCh1), {Qt::DisplayRole});
}

void MyTableModel::setSkip(int value) {
    RowData::m_skip = value;
    emit dataChanged(index(0, SignalCh0), index(15, MeasureCh1), {Qt::DisplayRole});
}

void MyTableModel::clearSelectedData() {
    for(bool enabled : m_rowEnabled) {
        if(enabled) {
            if(QMessageBox::question(reinterpret_cast<QWidget*>(parent()), "", "Вы действительно хотите отчистить выделенные каналы?", "Да", "Ну") == 0) {
                for(int row = 0; row < RowCount; ++row) {
                    if(m_rowEnabled[row]) {
                        m_data[row].clearData();
                        emit dataChanged(index(row, SignalCh0), index(row, MeasureCh1), {Qt::DisplayRole});
                    }
                }
            }
            return;
        }
    }
    QMessageBox::information(reinterpret_cast<QWidget*>(parent()), "", "Нет выделенных каналов!", "Ок");
}

void MyTableModel::clearData(int row) {
    if(0 > row || row > 15)
        return;
    m_data[row].clearData();
    emit dataChanged(index(row, SignalCh0), index(row, MeasureCh1), {Qt::DisplayRole});
}

QVector<double> MyTableModel::getData(int row, int pos) const {
    if(0 > row || row > 15)
        return {};
    return m_data[row].getData(pos);
}

RowData& MyTableModel::getData(int row) { return m_data[row]; }

const RowData& MyTableModel::getData(int row) const { return m_data[row]; }

void MyTableModel::addData(int row, int pos, double value) {
    if(0 > row || row > 15)
        return;
    m_data[row].addData(pos, value);
    (pos < 3) ? emit dataChanged(index(row, SignalCh0), index(row, MeasureCh0), {Qt::DisplayRole})
              : emit dataChanged(index(row, SignalCh1), index(row, MeasureCh1), {Qt::DisplayRole});
}

int MyTableModel::rowCount(const QModelIndex&) const { return RowCount; }

int MyTableModel::columnCount(const QModelIndex&) const { return ColumnCount; }

QVariant MyTableModel::data(const QModelIndex& index, int role) const {
    switch(role) {
    case Qt::DisplayRole:
        return m_data[index.row()].m_cellText[index.column()];
    case Qt::DecorationRole:
        switch(index.column()) {
        case MeasureCh0:
            return m_data[index.row()].icon[0];
        case MeasureCh1:
            return m_data[index.row()].icon[1];
        default:
            return QVariant();
        }
    case Qt::BackgroundColorRole:
        switch(index.column()) {
        case MeasureCh0:
            return m_data[index.row()].color[0];
        case MeasureCh1:
            return m_data[index.row()].color[1];
        default:
            return QVariant();
        }
    default:
        break;
    }
    return QVariant();
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    static const QStringList headerList({"Сигнал, Ом", "∆, мОм", "Канал АЦП 0", "Сигнал, Ом", "∆, мОм", "Канал АЦП 1"});

    switch(role) {
    case Qt::DisplayRole:
        if(orientation == Qt::Horizontal)
            return headerList.value(section);
        else
            return ++section;
    case Qt::TextAlignmentRole:
        if(orientation == Qt::Horizontal)
            return Qt::AlignCenter;
        else
            return {Qt::AlignRight | Qt::AlignVCenter};
    default:
        break;
    }
    return QVariant();
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex& index) const {
    return m_rowEnabled[index.row()] ? Qt::ItemIsEnabled : Qt::NoItemFlags;
}

void MyTableModel::save(const QString& backupName) {
    if(!backupName.isEmpty())
        m_backupName = QFileInfo(backupName).fileName().append(".bin");
    QFile file(m_backupName);
    if(file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for(int i = 0; i < RowCount; ++i)
            out << m_data[i];
        file.close();
    }
}

void MyTableModel::restore(const QString& backupName) {
    if(!backupName.isEmpty())
        m_backupName = QFileInfo(backupName).fileName().append(".bin");
    QFile file(m_backupName);
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        for(int i = 0; i < RowCount; ++i)
            in >> m_data[i];
        file.close();
    }
}
