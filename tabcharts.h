#pragma once

#include <QWidget>

class MeasureModel;
namespace QtCharts {
class QLineSeries;
}
namespace Ui {
class TabCharts;
}

class TabCharts : public QWidget {
    Q_OBJECT

public:
    explicit TabCharts(QWidget* parent = nullptr);
    ~TabCharts();

    void setModel(MeasureModel* model);
    void updateChart(int chNum);

private:
    Ui::TabCharts* ui;
    std::vector<QtCharts::QLineSeries*> series;
    MeasureModel* m_model;
};
