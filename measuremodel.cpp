#include "measuremodel.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

MeasureModel::MeasureModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_rowEnabled{
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
          Qt::Checked,
      } //
{
}

MeasureModel::~MeasureModel() {
    save();
}

void MeasureModel::setRowsEnabled(const std::vector<Qt::CheckState>& checkStates, Qt::Orientation) {
    for(int i = 0; i < RowCount; ++i) {
        if(m_rowEnabled[i] != checkStates[i]) {
            m_rowEnabled[i] = checkStates[i];
            emit dataChanged(index(i, 0), index(i, ColumnCount - 1), {Qt::DisplayRole});
        }
    }
}

void MeasureModel::setMax(double max) {
    RowData::m_max = max;
    emit dataChanged(index(0, SignalCh0), index(RowCount - 1, ColumnCount - 1), {Qt::DisplayRole});
}

void MeasureModel::setMin(double min) {
    RowData::m_min = min;
    emit dataChanged(index(0, SignalCh0), index(RowCount - 1, ColumnCount - 1), {Qt::DisplayRole});
}

void MeasureModel::setSkip(int skip) {
    RowData::m_skip = skip;
    for(auto&& row : m_data)
        for(int i : {0, 1, 2, 3, 4, 5})
            row.update(i);

    emit dataChanged(index(0, SignalCh0), index(RowCount - 1, ColumnCount - 1), {Qt::DisplayRole});
}

void MeasureModel::clearSelectedData() {
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

void MeasureModel::clearData(int row) {
    if(0 > row || row >= RowCount)
        return;
    m_data[row].clearData();
    emit dataChanged(index(row, 0), index(row, ColumnCount - 1), {Qt::DisplayRole});
    emit updateChart(row);
}

QVector<double> MeasureModel::getData(int row, int pos) const {
    if(0 > row || row >= RowCount)
        return {};
    return m_data[row].getData(pos);
}

RowData& MeasureModel::getData(int row) { return m_data[row]; }

const RowData& MeasureModel::getData(int row) const { return m_data[row]; }

void MeasureModel::addData(int row, int pos, double value) {
    static int lastRow = -1;
    if(0 > row || row > 15)
        return;
    m_data[row].addData(pos, value);
    (pos < 3) ? emit dataChanged(index(row, SignalCh0), index(row, MeasureCh0), {Qt::DisplayRole})
              : emit dataChanged(index(row, SignalCh1), index(row, MeasureCh1), {Qt::DisplayRole});
    emit updateChart(row);
    if(lastRow != row) {
        lastRow = row;
        save();
    }
}

int MeasureModel::rowCount(const QModelIndex&) const { return RowCount; }

int MeasureModel::columnCount(const QModelIndex&) const { return ColumnCount; }

QVariant MeasureModel::data(const QModelIndex& index, int role) const {
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

QVariant MeasureModel::headerData(int section, Qt::Orientation orientation, int role) const {
    static const QStringList headerList({"Сигнал, Ом", "∆, мОм", "Канал АЦП 0", "Сигнал, Ом", "∆, мОм", "Канал АЦП 1"});

    switch(role) {
    case Qt::DisplayRole:
        return (orientation == Qt::Horizontal) ? headerList.value(section)
                                               : QString::number(++section);
    case Qt::TextAlignmentRole:
        return (orientation == Qt::Horizontal) ? QVariant{Qt::AlignCenter}
                                               : QVariant{Qt::AlignRight | Qt::AlignVCenter};
    default:
        break;
    }
    return QVariant();
}

Qt::ItemFlags MeasureModel::flags(const QModelIndex& index) const {
    return m_rowEnabled[index.row()] ? Qt::ItemIsEnabled : Qt::NoItemFlags;
}

void MeasureModel::save(const QString& backupName) {
    if(!backupName.isEmpty())
        m_backupName = backupName + ".bin";
    QFile file(m_backupName);
    if(file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for(int i = 0; i < RowCount; ++i)
            out << m_data[i];
        file.close();
    } else {
        qDebug() << file.errorString();
    }
}

void MeasureModel::restore(const QString& backupName) {
    if(!backupName.isEmpty())
        m_backupName = backupName + ".bin";
    QFile file(m_backupName);
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        for(int i = 0; i < RowCount; ++i)
            in >> m_data[i];
        file.close();
        emit dataChanged(index(0, SignalCh0), index(RowCount - 1, ColumnCount - 1), {Qt::DisplayRole});
        qDebug(__FUNCTION__);
    } else {
        qDebug() << file.errorString();
    }
}
