#ifndef TABLE_H
#define TABLE_H

#include <QCheckBox>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QObject>
#include <QPaintEvent>
#include <QScrollBar>
#include <QTableView>
#include <QTableWidgetItem>
#include <QWidget>

#ifdef EXCEL
#include "excel.h"
#endif

class MyHeader;
class MyTableModel;
class QCheckBox;

class MyTable : public QTableView {
    Q_OBJECT
public:
    explicit MyTable(QWidget* parent = nullptr);
    ~MyTable();

    MyTableModel* model() const;
    QVector<bool> checkedRows() const;

    void enableDelta(bool checked);

    void loadFile(const QString& fileName);
    void printFile(const QString& fileName);
    void saveFile(const QString& fileName, const QString& asptNum, const QString& fio);
    void setCurrentFile(const QString& value);
    void setEnabledCheckBoxes(bool enabled);

signals:
    void updatePlot(int chNum);

private:
    MyHeader* m_header;
    MyTableModel* m_model;
    QCheckBox* c = nullptr;
    QString m_curFile;

#ifdef EXCEL
    Excel::Application* excel;
#endif
};

#endif // TABLE_H
