#pragma once

#include <QEventLoop>
#include <QWidget>

class Measure;
class MeasureModel;
namespace Ui {
class TabMeasure;
}

class TabMeasure : public QWidget {
    Q_OBJECT
    friend class MainWindow;

public:
    explicit TabMeasure(QWidget* parent = nullptr);
    ~TabMeasure();

private:
    Ui::TabMeasure* ui;
    Measure* measure = nullptr;
    QEventLoop eventLoop;

    void onPbStartStopClicked(bool checked = {});
    void measureFinished();
    void showMessage(const QString& msg, const QVector<int>& buttons, int* ret);
    void setEnabledMeasure(bool enabled);
    void setModel(MeasureModel* model);
};
