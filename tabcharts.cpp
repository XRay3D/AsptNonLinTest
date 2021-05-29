#include "tabcharts.h"
#include "measuremodel.h"
#include "ui_tabcharts.h"

#include <QGraphicsLayout>
#include <QLineSeries>

TabCharts::TabCharts(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TabCharts) //
{
    ui->setupUi(this);

    auto setupChart = [this](QChartView* chartView, const QString& name) {
        QChart* chart = new QChart();
        QLineSeries* s;

        s = new QLineSeries(chart);
        s->setName("0");
        chart->addSeries(s);
        series.emplace_back(s);

        s = new QLineSeries(chart);
        s->setName("1");
        chart->addSeries(s);
        series.emplace_back(s);

        chart->createDefaultAxes();
        chart->layout()->setContentsMargins(0, 0, 0, 0);
        chart->legend()->setAlignment(Qt::AlignRight);
        //        chart->legend()->setVisible(chartView == ui->chartView3);

        chart->setBackgroundRoundness(0);
        chart->setMargins(QMargins(0, 0, 0, 0));
        chart->setTheme(QChart::ChartThemeLight);
        chart->setTitle(name);

        chartView->setChart(chart);
        chartView->setRenderHints(QPainter::Antialiasing);
        chartView->setBackgroundBrush(Qt::white);
    };
    setupChart(ui->chartView1, "Сопротивление, Ом.\nR1");
    setupChart(ui->chartView2, "Сопротивление, Ом.\nR2");
    setupChart(ui->chartView3, "Сопротивление, Ом.\nR1+R2");

    connect(ui->cbxPlot, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &TabCharts::updateChart);
}

TabCharts::~TabCharts() { delete ui; }

void TabCharts::updateChart(int chNum) {
    ui->cbxPlot->setCurrentIndex(chNum);
    auto updateChart = [chNum, this](int AdcCh0, int AdcCh1, int SeriesR0, int SeriesR1, QChartView* graphicsView) {
        auto y1(m_model->getData(chNum, AdcCh0));
        auto y2(m_model->getData(chNum, AdcCh1));
        series[SeriesR0]->clear();
        series[SeriesR1]->clear();
        if(y1.size() > 1 || y2.size() > 1) {
            QPolygonF data(y1.size());
            double ctr{};
            for(auto var : y1)
                data[ctr] = {ctr + 1, var}, ++ctr;
            series[SeriesR0]->replace(data);
            ctr = {};
            for(auto var : y2)
                data[ctr] = {ctr + 1, var}, ++ctr;
            series[SeriesR1]->replace(data);

            graphicsView->chart()->axes(Qt::Horizontal).first()->setRange(1.0, qMax(y1.size(), y2.size()));
            y1.append(y2);
            auto [min, max] = std::ranges::minmax_element(y1);
            graphicsView->chart()->axes(Qt::Vertical).first()->setRange(*min, *max);
        } else {
            double d = 0.0;
            series[SeriesR0]->append(d, d);
            series[SeriesR1]->append(d, d);
        }
    };

    updateChart(0, 3, 0, 1, ui->chartView1);
    updateChart(1, 4, 2, 3, ui->chartView2);
    updateChart(2, 5, 4, 5, ui->chartView3);
}

void TabCharts::setModel(MeasureModel* model) {
    m_model = model;
    connect(model, &MeasureModel::updateChart, this, &TabCharts::updateChart);
}
