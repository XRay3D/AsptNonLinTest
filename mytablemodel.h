#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include "rowdata.h"
#include <QAbstractTableModel>

////////////////////////////////////////////////////////////////////////////////////////
/// \brief The MyTableModel class
///
class MyTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    MyTableModel(QObject* parent = Q_NULLPTR);
    void setRowsEnabled(const QVector<bool>& rowEnabled, int);

    void setMax(double value);
    void setMin(double value);
    void setSkip(int value);
    void clearSelectedData();
    void clearData(int row);

    QVector<double> getData(int row, int pos) const;
    RowData getData(int row) const;
    void addData(int row, int pos, double value);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    RowData m_data[16];
    bool m_rowEnabled[16];
};

#endif // MYTABLEMODEL_H
