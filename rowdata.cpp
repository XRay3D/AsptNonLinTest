#include "rowdata.h"

double RowData::m_max = 0.0001;
double RowData::m_min = 0.001;
int RowData::m_skip = 10;

RowData::RowData() { clearData(); }

double RowData::average(int pos) const { return m_average[pos]; }

void RowData::clearData()
{
    for (QVector<double>& data : m_data)
        data.clear();
    memset(m_average, 0, sizeof(m_average));
    for (int pos = 0; pos < 6; ++pos) {
        int i = (pos % 3);
        m_dataText[pos] = QString("R%1 = %2%3").arg(i + 1).arg(0.0, 0, 'f', 5).arg(i < 2 ? '\n' : ' ').replace('.', ',');
        m_deltaText[pos] = QString("%1%2").arg(0.0, 0, 'g', 3).arg(i < 2 ? '\n' : ' ').replace('.', ',');
        if (pos < 3) {
            m_cellText[MeasureCh0] = QString("%1").arg(0.0, 0, 'f', 6).replace('.', ',');
            color[CH0] = QColor::fromHsv(0, 50, 255);
            icon[CH0] = QIcon("icon1.svg");
            m_cellText[SignalCh0] = m_dataText[R1_CH0] + m_dataText[R2_CH0] + m_dataText[R3_CH0];
            m_cellText[MeasureDeltaCh0] = m_deltaText[R1_CH0] + m_deltaText[R2_CH0] + m_deltaText[R3_CH0];
        } else {
            m_cellText[MeasureCh1] = QString("%1").arg(0.0, 0, 'f', 6).replace('.', ',');
            color[CH1] = QColor::fromHsv(0, 50, 255);
            icon[CH1] = QIcon("icon1.svg");
            m_cellText[SignalCh1] = m_dataText[R1_CH1] + m_dataText[R2_CH1] + m_dataText[R3_CH1];
            m_cellText[MeasureDeltaCh1] = m_deltaText[R1_CH1] + m_deltaText[R2_CH1] + m_deltaText[R3_CH1];
        }
    }
}

QVector<double> RowData::getData(int pos) const { return m_data[pos]; }

void RowData::addData(const int pos, double val)
{
    m_data[pos].append(val);
    update(pos);
}

void RowData::update(const int pos)
{
    double delta;
    double min = 0.0;
    double max = 0.0;
    if (m_data[pos].count()) {
        QVector<double> v(m_data[pos]);
        std::sort(v.begin(), v.end());
        min = v.first();
        max = v.last();
        m_average[pos] = 0;
        if (v.count() > m_skip) {
            for (int i = static_cast<int>(ceil(m_skip * 0.5)); i < (m_data[pos].count() - floor(m_skip * 0.5)); ++i) {
                m_average[pos] += v[i];
            }
            m_average[pos] /= v.count() - m_skip;
        } else {
            for (double val : v) {
                m_average[pos] += val;
            }
            m_average[pos] /= v.count();
        }
    }
    ///////////////////////
    const int i = (pos % 3);
    m_dataText[pos] = QString("R%1 = %2%3").arg(i + 1).arg(m_average[pos], 0, 'f', 5).arg(i < 2 ? '\n' : ' ').replace('.', ',');
    m_deltaText[pos] = QString("%1%2").arg((max - min) * 1000.0, 0, 'g', 3).arg(i < 2 ? '\n' : ' ').replace('.', ',');

    auto decor = [&](int ch = CH0) {
        delta = abs(delta);
        if (delta > m_max || delta == 0.0) {
            color[ch] = QColor::fromHsv(0, 50, 255);
            icon[ch] = QIcon("icon1.svg");
        } else if (delta < m_min) {
            color[ch] = QColor::fromHsv(120, 50, 255);
            icon[ch] = QIcon("icon2.svg");
        } else {
            color[ch] = QColor::fromHsv(120 - static_cast<int>((120 / (m_max - m_min)) * (delta - m_min)), 50, 255);
            icon[ch] = QIcon("icon3.svg");
        }
    };

    if (pos < 3) {
        delta = m_average[R1_CH0] + m_average[R2_CH0] - m_average[R3_CH0];
        m_cellText[MeasureCh0] = QString("%1%2").arg(delta < 0 ? "" : "").arg(delta, 0, 'f', 6).replace('.', ',');
        m_cellText[SignalCh0] = m_dataText[R1_CH0] + m_dataText[R2_CH0] + m_dataText[R3_CH0];
        m_cellText[MeasureDeltaCh0] = m_deltaText[R1_CH0] + m_deltaText[R2_CH0] + m_deltaText[R3_CH0];
        decor(CH0);
    } else {
        delta = m_average[R1_CH1] + m_average[R2_CH1] - m_average[R3_CH1];
        m_cellText[MeasureCh1] = QString("%1%2").arg(delta < 0 ? "" : "").arg(delta, 0, 'f', 6).replace('.', ',');
        m_cellText[SignalCh1] = m_dataText[R1_CH1] + m_dataText[R2_CH1] + m_dataText[R3_CH1];
        m_cellText[MeasureDeltaCh1] = m_deltaText[R1_CH1] + m_deltaText[R2_CH1] + m_deltaText[R3_CH1];
        decor(CH1);
    }
}
