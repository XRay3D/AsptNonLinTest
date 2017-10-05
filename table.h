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

    void LoadFile(const QString& fileName);
    void SaveFile(const QString& fileName, const QString& asptNum, const QString& fio);
    void PrintFile(const QString& fileName);

    void CheckUncheckAll(bool checked);
    void CheckPpm(bool checked);
    void CheckDelta(bool checked);
    void clearSelectedData();
    void clearData(int row);
    void Update(int row);
    void Update(int row, int pos);

    void setMax(double value);
    void setMin(double value);
    void setResistors(const QVector<double>&& value);
    void setCurFile(const QString& value);
    void setEnableRow(bool fl);

    QVector<double> getData(int row, int pos) const;
    void addData(int row, int pos, double val);
    void setSkip(int value);

signals:
    void updatePlot(int chNum);

public slots:

private:
    QVector<double> resistors;
    double max = 0.0001;
    double min = 0.001;
    int skip = 0;
#ifdef EXCEL
    Excel::Application* excel;
#endif
    QString curFile;

    QVector<QVector<QVector<double> > > data;
    QVector<QVector<double> > average;
    QVector<QVector<QVector<QString> > > cellText;
    bool dataChanged;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;
};

#endif // TABLE_H
