#include "mytablemodel.h"

MyTableModel::MyTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_rowEnabled(16, false)
{
}

void MyTableModel::stateChanged(const QVector< bool>& rowEnabled, int)
{
    for (int i = 0; i < 16; ++i) {
        if (m_rowEnabled[i] != rowEnabled[i]) {
        }
    }
}

int MyTableModel::rowCount(const QModelIndex& parent) const
{
    return 16;
}

int MyTableModel::columnCount(const QModelIndex& parent) const
{
    return 8;
}

QVariant MyTableModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static const QStringList headerList({ //"№",
        "Сигнал, Ом", "∆, мОм", "PPM", "Канал АЦП 0",
        "Сигнал, Ом", "∆, мОм", "PPM", "Канал АЦП 1" });

    switch (role) {
    case Qt::DisplayRole:
        if (orientation == Qt::Horizontal)
            return headerList.value(section);
        else
            return ++section;
    case Qt::TextAlignmentRole:
        if (orientation == Qt::Horizontal)
            return Qt::AlignCenter;
        else
            return Qt::AlignRight;
    default:
        break;
    }
    return QVariant();
}
