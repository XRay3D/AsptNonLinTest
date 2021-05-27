#pragma once

#include "rowdata.h"
#include <QAbstractTableModel>

////////////////////////////////////////////////////////////////////////////////////////
/// \brief The MyTableModel class
///
class MeasureModel : public QAbstractTableModel {
    Q_OBJECT

public:
    MeasureModel(QObject* parent = nullptr);
    ~MeasureModel() override;
    void setRowsEnabled(const std::vector<Qt::CheckState>& checkStates, Qt::Orientation);

    void setMax(double max);
    void setMin(double min);
    void setSkip(int skip);
    void clearSelectedData();
    void clearData(int row);

    QVector<double> getData(int row, int pos) const;
    RowData& getData(int row);
    const RowData& getData(int row) const;
    void addData(int row, int pos, double value);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& = {}) const override;
    int columnCount(const QModelIndex& = {}) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void save(const QString& backupName = {});
    void restore(const QString& backupName = {});

private:
    QString m_backupName;
    RowData m_data[16];
    bool m_rowEnabled[16];
};
