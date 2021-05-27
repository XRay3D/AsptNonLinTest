#include "measuremodel.h"

#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

MeasureModel::MeasureModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_rowEnabled { false }
{
    restore();
}

MeasureModel::~MeasureModel()
{
    save();
}

void MeasureModel::setRowsEnabled(const std::vector<Qt::CheckState>& checkStates, Qt::Orientation)
{
    for (int i = 0; i < RowCount; ++i) {
        if (m_rowEnabled[i] != checkStates[i]) {
            m_rowEnabled[i] = checkStates[i];
            emit dataChanged(index(i, SignalCh0), index(i, MeasureCh1), { Qt::DisplayRole });
        }
    }
}

void MeasureModel::setMax(double max)
{
    RowData::m_max = max;
    emit dataChanged(index(0, SignalCh0), index(15, MeasureCh1), { Qt::DisplayRole });
}

void MeasureModel::setMin(double min)
{
    RowData::m_min = min;
    emit dataChanged(index(0, SignalCh0), index(15, MeasureCh1), { Qt::DisplayRole });
}

void MeasureModel::setSkip(int skip)
{
    RowData::m_skip = skip;
    emit dataChanged(index(0, SignalCh0), index(15, MeasureCh1), { Qt::DisplayRole });
}

void MeasureModel::clearSelectedData()
{
    for (bool enabled : m_rowEnabled) {
        if (enabled) {
            if (QMessageBox::question(reinterpret_cast<QWidget*>(parent()), "", "Вы действительно хотите отчистить выделенные каналы?", "Да", "Ну") == 0) {
                for (int row = 0; row < RowCount; ++row) {
                    if (m_rowEnabled[row]) {
                        m_data[row].clearData();
                        emit dataChanged(index(row, SignalCh0), index(row, MeasureCh1), { Qt::DisplayRole });
                    }
                }
            }
            return;
        }
    }
    QMessageBox::information(reinterpret_cast<QWidget*>(parent()), "", "Нет выделенных каналов!", "Ок");
}

void MeasureModel::clearData(int row)
{
    if (0 > row || row > 15)
        return;
    m_data[row].clearData();
    emit dataChanged(index(row, SignalCh0), index(row, MeasureCh1), { Qt::DisplayRole });
}

QVector<double> MeasureModel::getData(int row, int pos) const
{
    if (0 > row || row > 15)
        return {};
    return m_data[row].getData(pos);
}

RowData& MeasureModel::getData(int row) { return m_data[row]; }

const RowData& MeasureModel::getData(int row) const { return m_data[row]; }

void MeasureModel::addData(int row, int pos, double value)
{
    if (0 > row || row > 15)
        return;
    m_data[row].addData(pos, value);
    (pos < 3) ? emit dataChanged(index(row, SignalCh0), index(row, MeasureCh0), { Qt::DisplayRole })
              : emit dataChanged(index(row, SignalCh1), index(row, MeasureCh1), { Qt::DisplayRole });
}

int MeasureModel::rowCount(const QModelIndex&) const { return RowCount; }

int MeasureModel::columnCount(const QModelIndex&) const { return ColumnCount; }

QVariant MeasureModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return m_data[index.row()].m_cellText[index.column()];
    case Qt::DecorationRole:
        switch (index.column()) {
        case MeasureCh0:
            return m_data[index.row()].icon[0];
        case MeasureCh1:
            return m_data[index.row()].icon[1];
        default:
            return QVariant();
        }
    case Qt::BackgroundColorRole:
        switch (index.column()) {
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

QVariant MeasureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QStringList headerList({ "Сигнал, Ом", "∆, мОм", "Канал АЦП 0", "Сигнал, Ом", "∆, мОм", "Канал АЦП 1" });

    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return headerList.value(section);
        else
            return ++section;
    case Qt::TextAlignmentRole:
        if (orientation == Qt::Horizontal)
            return { Qt::AlignCenter };
        else
            return { Qt::AlignRight | Qt::AlignVCenter };
    default:
        break;
    }
    return QVariant();
}

Qt::ItemFlags MeasureModel::flags(const QModelIndex& index) const
{
    return m_rowEnabled[index.row()] ? Qt::ItemIsEnabled : Qt::NoItemFlags;
}

void MeasureModel::save(const QString& backupName)
{
    if (!backupName.isEmpty())
        m_backupName = QFileInfo(backupName).fileName().split(' ').front().append(".bin");
    QFile file(m_backupName);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for (int i = 0; i < RowCount; ++i)
            out << m_data[i];
        file.close();
    }
}

void MeasureModel::restore(const QString& backupName)
{
    if (!backupName.isEmpty())
        m_backupName = QFileInfo(backupName).fileName().split(' ').front().append(".bin");
    QFile file(m_backupName);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        for (int i = 0; i < RowCount; ++i)
            in >> m_data[i];
        file.close();
    }
}
