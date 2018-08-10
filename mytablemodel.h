#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include "mytablemodel.h"

#include <QAbstractTableModel>

class MyTableModel : public QAbstractTableModel {
public:
    MyTableModel(QObject* parent = Q_NULLPTR);
    void stateChanged(const QVector<bool>& rowEnabled, int);
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QVector<bool> m_rowEnabled;
};

#endif // MYTABLEMODEL_H
