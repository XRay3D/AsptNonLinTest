#pragma once

#include "tableparams.h"
#include <QDataStream>
#include <QIcon>

class RowData {
    friend inline QDataStream& operator>>(QDataStream& s, RowData& d)
    {
        d.clearData();
        s >> m_max;
        s >> m_min;
        s >> m_skip;
        for (auto& var : d.m_data) {
            size_t size {};
            var.resize(size);
            for (auto& var : var)
                s >> var;
        }
        for (int i = 0; i < 6; ++i)
            d.update(i);
        return s;
    }

    friend inline QDataStream& operator<<(QDataStream& s, const RowData& d)
    {
        s << m_max;
        s << m_min;
        s << m_skip;
        for (auto& var : d.m_data) {
            s << var.size();
            for (auto& var : var)
                s << var;
        }
        return s;
    }
    void update(const int pos);

    std::vector<double> m_data[ColumnCount];
    double m_average[ColumnCount];
    std::string m_dataText[ColumnCount];
    std::string m_deltaText[ColumnCount];

    enum {
        R1_CH0,
        R2_CH0,
        R3_CH0,
        R1_CH1,
        R2_CH1,
        R3_CH1
    };

    enum {
        AdcCh0,
        AdcCh1,
        ChCount
    };

public:
    RowData();
    static double m_max; // = 0.0001;
    static double m_min; // = 0.001;
    static int m_skip; // = 0;
    QString m_cellText[ColumnCount];
    QIcon icon[ChCount];
    QColor color[ChCount];

    double average(int pos) const;
    void clearData();
    QVector<double> getData(int pos) const;
    void addData(const int pos, double val);
};
