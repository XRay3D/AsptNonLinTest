#include "tabmeasure.h"
#include "mainwindow.h"
#include "measuremodel.h"
#include "ui_tabmeasure.h"

#include <QMessageBox>
#include <measuring_interface/measure.h>
#include <measuring_interface/mi.h>

TabMeasure::TabMeasure(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TabMeasure) //
{
    ui->setupUi(this);
    ui->pbClear->setIcon(QIcon::fromTheme("list-remove"));
    connect(ui->pbStartMeasure, &QPushButton::clicked, this, &TabMeasure::onPbStartStopClicked);
    connect(ui->chbxDelta, &QCheckBox::toggled, ui->tableView, &MyTable::enableDelta);
}

TabMeasure::~TabMeasure() { delete ui; }

void TabMeasure::onPbStartStopClicked(bool checked) {
    if(checked) {
        int measuresCount = MainWindow::instance()->sbxMeasNum->value() + MainWindow::instance()->sbxSkipMeasNum->value();
        {
            int progressMax{};
            for(auto enabled : MainWindow::instance()->model->rowEnabled())
                if(enabled)
                    ++progressMax;
            progressMax *= measuresCount * 6;
            MainWindow::instance()->setProgressMax(progressMax);
        }

        measure = new Measure(MainWindow::instance()->model, measuresCount);
        connect(measure, &Measure::showMessage, this, &TabMeasure::showMessage);
        connect(measure, &Measure::updateProgressVal, MainWindow::instance(), &MainWindow::setProgressVal);
        connect(measure, &QThread::finished, measure, &QObject::deleteLater);
        connect(measure, &QThread::finished, this, &TabMeasure::measureFinished);
        MI::aspt()->ping(MainWindow::instance()->cbxAspt->currentText());
        MI::upn()->ping(MainWindow::instance()->cbxUpn->currentText());
        measure->start();
    } else {
        if(sender() == ui->pbStartMeasure) {
            if(eventLoop.isRunning())
                return;
            connect(measure, &QThread::finished, [this] { eventLoop.exit(); });
            measure->requestInterruption();
            eventLoop.exec();
        }
        if(sender() == measure) {
            qDebug(__FUNCTION__);
            MainWindow::instance()->setProgressVal(0);
            measure = nullptr;
        }
    }
    ui->pbStartMeasure->setChecked(checked);

    ui->pbStartMeasure->setIcon(QIcon::fromTheme(checked ? "media-playback-stop"
                                                         : "media-playback-start"));
    ui->pbStartMeasure->setText(checked ? "Остановить проверку"
                                        : "Начать проверку");

    MainWindow::instance()->menuBar()->setEnabled(!checked);
    MainWindow::instance()->tabDevices->setEnabled(!checked);
    MainWindow::instance()->tabUpn->setEnabled(!checked);
    ui->pbClear->setEnabled(!checked);
    ui->tableView->setEnabledCheckBoxes(!checked);
}

void TabMeasure::measureFinished() { onPbStartStopClicked(); }

void TabMeasure::showMessage(const QString& msg, const QVector<int>& buttons, int* ret) {
    MainWindow::instance()->taskbarProgress->stop();
    *ret = QMessageBox::information(this, MainWindow::instance()->leAsptSerNum->text(), msg, buttons.value(0), buttons.value(1), buttons.value(2));
    MainWindow::instance()->taskbarProgress->resume();
    measure->resume();
}

void TabMeasure::setEnabledMeasure(bool enabled) { ui->pbStartMeasure->setEnabled(enabled); }

void TabMeasure::setModel(MeasureModel* model) {
    ui->tableView->setModel(model);
    connect(ui->pbClear, &QPushButton::clicked, model, &MeasureModel::clearSelectedData);
}
