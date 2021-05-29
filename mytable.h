#pragma once

#include <QTableView>

class MyHeader;
class MeasureModel;
class QCheckBox;

class MyTable : public QTableView {
    Q_OBJECT
public:
    explicit MyTable(QWidget* parent = nullptr);
    ~MyTable();

    std::vector<Qt::CheckState> checkedRows() const;

    void enableDelta(bool checked);

    void setEnabledCheckBoxes(bool enabled);
    void setModel(QAbstractItemModel* model) override;

private:
    MyHeader* m_header;
    QCheckBox* cornerCheckBox = nullptr;
};
