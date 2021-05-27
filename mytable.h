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

    MeasureModel* model() const;
    std::vector<Qt::CheckState> checkedRows() const;

    void enableDelta(bool checked);

    void openFile(const QString& fileName);
    void printFile(const QString& fileName);
    void saveFile(const QString& fileName);
    void saveFile(const QString& fileName, const QString& asptNum, const QString& fio);

    void setEnabledCheckBoxes(bool enabled);

signals:
    void updatePlot(int chNum);

private:
    MyHeader* m_header;
    MeasureModel* m_model;
    QCheckBox* cornerCheckBox = nullptr;
};
