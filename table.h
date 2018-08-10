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

class MyHeader : public QHeaderView {
    Q_OBJECT
public:
    MyHeader(Qt::Orientation orientation, QWidget* parent = 0);
    ~MyHeader();
    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;

signals:
    void stateChanged(const QVector<bool>&, int);

private:
    QVector<bool> m_isOn;

    // QWidget interface
public:
    QSize sizeHint() const override;
};

class Table : public QTableView {
    Q_OBJECT
public:
    explicit Table(QWidget* parent = 0);
    ~Table();

    void loadFile(const QString& fileName);
    void saveFile(const QString& fileName, const QString& asptNum, const QString& fio);
    void printFile(const QString& fileName);

    void checkUncheckAll(bool checked);

    void clearSelectedData();
    void clearData(int row);
    void updateRow(int row);
    void updateRow(int row, int pos);

    void setMax(double value);
    void setMin(double value);
    void setResistorsValue(const QVector<double>&& value);
    void setCurrentFile(const QString& value);

    void enableRow(bool checked);
    void enablePpm(bool checked);
    void enableDelta(bool checked);

    QVector<double> getData(int row, int pos) const;
    void addData(int row, int pos, double value);
    void setSkip(int value);

signals:
    void updatePlot(int chNum);

private:
    bool m_dataChanged;

    double m_max = 0.0001;
    double m_min = 0.001;

    int m_skip = 0;

    QString m_curFile;

    QVector<double> m_resistors;
    QVector<QVector<double>> m_average;
    QVector<QVector<QVector<double>>> m_data;
    QVector<QVector<QVector<QString>>> m_cellText;

#ifdef EXCEL
    Excel::Application* excel;
#endif
    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
};

#endif // TABLE_H
