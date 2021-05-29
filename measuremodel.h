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
    auto rowEnabled() const { return m_rowEnabled; }
    void setMax(double max);
    void setMin(double min);
    void setSkip(int skip);
    void clearSelectedData();
    void clearData(int row);

    QVector<double> getData(int row, int pos) const;
    RowData& getData(int row);
    const RowData& getData(int row) const;
    void addData(int row, int pos, double value);

signals:
    void updateChart(int row);
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
    RowData m_data[RowCount];
    struct
    {
        Qt::CheckState data[RowCount];
        auto& operator[](int i) { return data[i]; }
        auto& operator[](int i) const { return data[i]; }
        auto begin() { return data; }
        auto end() { return data + RowCount; }
    } m_rowEnabled;
    //    Qt::CheckState m_rowEnabled[RowCount];
};
