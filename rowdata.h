#ifndef ROWDATA_H
#define ROWDATA_H

#include "tableparams.h"
#include <QIcon>

class RowData {
public:
    RowData();
    static double m_max; // = 0.0001;
    static double m_min; // = 0.001;
    static int m_skip; // = 0;

    QString m_cellText[ColumnCount];

    QIcon icon[2];
    QColor color[2];

    double average(int pos) const;

    void clearData();

    QVector<double> getData(int pos) const;

    void addData(const int pos, double val);

private:
    QVector<double> m_data[6];
    double m_average[6];
    QString m_dataText[6];
    QString m_deltaText[6];

    enum {
        R1_CH0,
        R2_CH0,
        R3_CH0,
        R1_CH1,
        R2_CH1,
        R3_CH1
    };

    enum {
        CH0,
        CH1
    };
};

#endif // ROWDATA_H
