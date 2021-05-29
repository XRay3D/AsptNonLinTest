#pragma once

#include <QDataStream>
#include <QDebug>
#include <QIcon>

enum ColumnType {
    SignalCh0,
    MeasureDeltaCh0,
    //MeasurePpmCh0,
    MeasureCh0,
    SignalCh1,
    MeasureDeltaCh1,
    //MeasurePpmCh1,
    MeasureCh1,
    ColumnCount

};

enum { RowCount = 16 };

template <typename Container>
QDataStream& operator>>(QDataStream& stream, Container& c) {
    c.clear();
    quint32 n;
    stream >> n;
    c.reserve(n);
    for(quint32 i = 0; i < n; ++i) {
        typename Container::value_type t;
        stream >> t;
        if(stream.status() != QDataStream::Ok) {
            c.clear();
            break;
        }
        c.emplace_back(t);
    }
    return stream;
}

template <typename Container>
QDataStream& operator<<(QDataStream& stream, const Container& c) {
    stream << quint32(c.size());
    for(const typename Container::value_type& t : c)
        stream << t;
    return stream;
}

class RowData {
    friend inline QDataStream& operator>>(QDataStream& stream, RowData& d) {
        d.clearData();
        stream >> m_max;
        stream >> m_min;
        stream >> m_skip;
        for(auto& var : d.m_data)
            stream >> var;

        for(int i = 0; i < 6; ++i)
            d.update(i);
        return stream;
    }

    friend inline QDataStream& operator<<(QDataStream& stream, const RowData& d) {
        stream << m_max;
        stream << m_min;
        stream << m_skip;
        for(auto& var : d.m_data)
            stream << var;

        return stream;
    }

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

    static inline double m_max = 0.0001;
    static inline double m_min = 0.001;
    static inline uint m_skip = 10;

    QString m_cellText[ColumnCount];
    QIcon icon[ChCount];
    QColor color[ChCount];

    QVector<double> getData(int pos) const;
    double average(int pos) const;
    void addData(const int pos, double val);
    void clearData();
    void update(const int pos);
};
