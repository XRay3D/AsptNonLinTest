#ifndef TABLE_H
#define TABLE_H

#include <QCheckBox>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QObject>
#include <QPaintEvent>
#include <QScrollBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

#ifdef EXCEL
#include "excel.h"
#endif

class TABLE : public QTableWidget {
    Q_OBJECT
public:
    explicit TABLE(QWidget* parent = 0);
    ~TABLE();

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
    QVector<QVector<double> > m_average;
    QVector<QVector<QVector<double> > > m_data;
    QVector<QVector<QVector<QString> > > m_cellText;

#ifdef EXCEL
    Excel::Application* excel;
#endif
    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
};

#endif // TABLE_H
