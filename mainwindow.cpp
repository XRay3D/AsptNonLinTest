#include "mainwindow.h"
#include "measuremodel.h"
#include "measuring_interface/devfinder.h"
#include "ui_mainwindow.h"
#include <QChart>
#include <QEventLoop>
#include <QLineSeries>
#include <QtWidgets>
#include <thread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    //    , Start(QStringLiteral(":/res/image2.png") /*QIcon::fromTheme("draw-triangle2")*/)
    //    , Stop(QStringLiteral(":/res/image3.png") /*QIcon::fromTheme("draw-rectangle")*/)
    , recentFiles(this, "proto")
{

    QIcon::setThemeName("breeze_xr");

    ui->setupUi(this);

    auto setupChart = [&](QChartView* chartView, const QString& name) {
        QChart* chart = new QChart();
        QLineSeries* s;

        s = new QLineSeries(chart);
        s->setName("Подканал 0");
        chart->addSeries(s);
        series.append(s);

        s = new QLineSeries(chart);
        s->setName("Подканал 1");
        chart->addSeries(s);
        series.append(s);

        chart->setTitle(name);
        chart->legend()->setAlignment(Qt::AlignRight);
        chart->setMargins(QMargins(0, 0, 0, 0));
        chart->createDefaultAxes();
        chart->setTheme(QChart::ChartThemeDark);
        chart->layout()->setContentsMargins(0, 0, 0, 0);
        chart->setBackgroundRoundness(0);

        chartView->setChart(chart);
        chartView->setRenderHints(QPainter::Antialiasing);
        chartView->setBackgroundBrush(Qt::white);
    };
    setupChart(ui->chartView1, "Сопротивление, Ом.\nR1");
    setupChart(ui->chartView2, "Сопротивление, Ом.\nR2");
    setupChart(ui->chartView3, "Сопротивление, Ом.\nR1+R2");

    connectObjects();
    loadSettings();

    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    emit stopWork(3);
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    saveSettings();
    event->accept();
}

void MainWindow::updatePlot(int chNum)
{
    auto updateChart = [chNum, this](int AdcCh0, int AdcCh1, int SeriesR0, int SeriesR1, QChartView* graphicsView) {
        auto y1(ui->table->model()->getData(chNum, AdcCh0));
        auto y2(ui->table->model()->getData(chNum, AdcCh1));
        series[SeriesR0]->clear();
        series[SeriesR1]->clear();
        if (y1.size() > 1 || y2.size() > 1) {
            QPolygonF data(y1.size());
            double ctr {};
            for (auto var : y1)
                data[ctr] = { ctr + 1, var }, ++ctr;
            series[SeriesR0]->replace(data);
            ctr = {};
            for (auto var : y2)
                data[ctr] = { ctr + 1, var }, ++ctr;
            series[SeriesR1]->replace(data);
            y1.append(y2);

            auto [min, max] = std::ranges::minmax_element(y1);

            graphicsView->chart()->axes(Qt::Vertical).first()->setRange(*min, *max);
            graphicsView->chart()->axes(Qt::Horizontal).first()->setRange(1.0, qMax(y1.size(), y2.size()));
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

void MainWindow::on_pbUpnRead_clicked()
{
    MI::upn()->ping(ui->cbxUpn->currentText());
    if (MI::upn()->readResistorValue()) {
        ui->dsbxUpnR1->setValue(MI::upn()->resistors()[0]);
        ui->dsbxUpnR2->setValue(MI::upn()->resistors()[1]);
        ui->dsbxUpnR3->setValue(MI::upn()->resistors()[2]);
        ui->sbxUpnSerNum->setValue(static_cast<int>(MI::upn()->resistors()[5]));
        QMessageBox::information(
            this, tr("УПН"), tr("Значения из УПН успешно считаны!"), "Хоошо :-)");
    } else {
        QMessageBox::critical(this, tr("УПН"), tr("Не удалось прочитать значения из УПН!"), "Плохо :-(");
    }
}

void MainWindow::on_pbUpnWrite_clicked()
{
    MI::upn()->ping(ui->cbxUpn->currentText());
    if (QMessageBox::question(this, tr("УПН"), tr("Вы действительно хотите записать новые значения в УПН?"), "Да", "Нет", "", 1, 1) == 0) {
        if (MI::upn()->writeResistorValue(
                { ui->dsbxUpnR1->value(),
                    ui->dsbxUpnR2->value(),
                    ui->dsbxUpnR3->value(),
                    0.0,
                    0.0,
                    double(ui->sbxUpnSerNum->value()) })) {
            QMessageBox::information(this, tr("УПН"), tr("Новые значения в УПН успешно записаны!"), "Хоошо :-)");
            ui->cbxUpn->setItemData(ui->cbxUpn->currentIndex(), QVariant::fromValue(MI::upn()->resistors()));
            //            ui->table->model()->setResistorsValue(ui->cbxUpn->currentData().value<QVector<double>>());
            ui->leUpnSerNum->setText(ui->sbxUpnSerNum->text());
        } else {
            QMessageBox::critical(this, tr("УПН"), tr("Не удалось записать новые значения в УПН!"), "Плохо :-(");
        }
    }
}

void MainWindow::setProgressVisible(bool fl)
{
    progressBar.setVisible(fl);
    lbRemainingTime.setVisible(fl);
    if (fl) {
        taskbarProgress->show();
        progressBar.setValue(0);
        lbRemainingTime.setText("Осталось: ?");
    } else
        taskbarProgress->hide();
}

void MainWindow::currentIndexChanged(int index)
{
    index = ui->cbxAspt->itemData(ui->cbxAspt->currentIndex()).toInt();
    if (index > 0)
        ui->leAsptSerNum->setText(QString("%1").arg(index).insert(2, '-'));
    else
        ui->leAsptSerNum->clear();
    QVector<double> r(ui->cbxUpn->currentData().value<QVector<double>>());
    if (r.size() == 6) {
        ui->leUpnSerNum->setText(QString("%1").arg(r[5]));
        ui->dsbxUpnR1->setValue(r[0]);
        ui->dsbxUpnR2->setValue(r[1]);
        ui->dsbxUpnR3->setValue(r[2]);
        ui->sbxUpnSerNum->setValue(static_cast<int>(r[5]));
        ui->tabUpn->setEnabled(true);
        //        ui->table->model()->setResistorsValue(ui->cbxUpn->currentData().value<QVector<double>>());
    } else {
        ui->leUpnSerNum->clear();
        ui->dsbxUpnR1->setValue(0.0);
        ui->dsbxUpnR2->setValue(0.0);
        ui->dsbxUpnR3->setValue(0.0);
        ui->sbxUpnSerNum->setValue(0);
        ui->tabUpn->setEnabled(false);
    }
}

void MainWindow::setProgressMax(int max)
{
    progressBar.setMaximum(max);
    taskbarProgress->setRange(0, max);
}

void MainWindow::setProgressVal(int val)
{
    setProgressVisible(val);
    progressBar.setValue(val);
    taskbarProgress->resume();
    taskbarProgress->setValue(val);
}

void MainWindow::connectObjects()
{
    /***************** Thread *****************/
    connect(this, &MainWindow::goMeasure, MI::measure(), &Measure::measure);
    connect(this, &MainWindow::stopWork, MI::measure(), &Measure::stopWork);

    connect(MI::measure(), &Measure::measureReady, this, &MainWindow::handleMeasure);
    connect(MI::measure(), &Measure::doMessage, this, &MainWindow::handleMessage);

    /***************** table->model() *****************/

    connect(ui->leAsptSerNum, &QLineEdit::textChanged, ui->table->model(), &MeasureModel::restore);

    connect(ui->table, &MyTable::updatePlot, ui->cbxPlot, &QComboBox::setCurrentIndex);
    connect(ui->table, &MyTable::updatePlot, this, &MainWindow::updatePlot);
    connect(ui->cbxPlot, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::updatePlot);
    connect(ui->sbxSkipMeasNum, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->table->model(), &MeasureModel::setSkip);

    connect(ui->chbxDelta, &QCheckBox::toggled, ui->table, &MyTable::enableDelta);
    connect(ui->pbClear, &QPushButton::clicked, ui->table->model(), &MeasureModel::clearSelectedData);

    connect(ui->cbxAspt, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);
    connect(ui->cbxUpn, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);
    connect(ui->cbxUpn, qOverload<const QString&>(&QComboBox::currentIndexChanged), [](const QString& portName) { MI::upn()->ping(portName); });

    connect(ui->pbFindDevices, &QPushButton::clicked, [=](bool checked) {
        if (checked) {
            finder = new DevFinder(ui->cbxAspt, ui->cbxUpn);
            connect(finder, &DevFinder::updateProgressVal, this, &MainWindow::setProgressVal);
            connect(finder, &DevFinder::updateProgressMax, this, &MainWindow::setProgressMax);
            connect(finder, &QThread::finished, finder, &QObject::deleteLater);
            connect(finder, &QThread::finished, this, &MainWindow::devFinderFinished);
            finder->start();
            ui->pbFindDevices->setIcon(Stop);
            ui->pbFindDevices->setText(tr("Остановить."));
            ui->tabWidget->tabBar()->setEnabled(false);
        } else {
            if (finder) {
                finder->requestInterruption();
                finder->quit();
                finder->wait();
            }
        }
    });

    ui->tabUpn->setEnabled(false);

    connect(this, &MainWindow::setResistor, MI::upn(), &Upn::setResistor);
    connect(ui->pbUpnR1Ch0, &QPushButton::clicked, [this] { emit setResistor(0); });
    connect(ui->pbUpnR2Ch0, &QPushButton::clicked, [this] { emit setResistor(1); });
    connect(ui->pbUpnR3Ch0, &QPushButton::clicked, [this] { emit setResistor(2); });
    connect(ui->pbUpnR1Ch1, &QPushButton::clicked, [this] { emit setResistor(3); });
    connect(ui->pbUpnR2Ch1, &QPushButton::clicked, [this] { emit setResistor(4); });
    connect(ui->pbUpnR3Ch1, &QPushButton::clicked, [this] { emit setResistor(5); });

    connect(ui->dsbxMin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double arg1) {
        if (ui->dsbxMax->value() < arg1)
            ui->dsbxMax->setValue(arg1);
        ui->table->model()->setMin(arg1);
    });

    connect(ui->dsbxMax, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double arg1) {
        if (ui->dsbxMin->value() > arg1)
            ui->dsbxMin->setValue(arg1);
        ui->table->model()->setMax(arg1);
    });
    //    connect(ui->leFio, &QLineEdit::textChanged, [this](const QString& text) { ui->pbStartStop->setEnabled(text == "skip"); });
    connect(ui->pbStartStop, &QPushButton::clicked, [=](bool checked) {
        if (checked) {
            channels.clear();
            for (int row = 0; row < 16; ++row) {
                if (ui->table->checkedRows()[row])
                    channels.append(qMakePair(int(row), int(3)));
            }
            if (channels.isEmpty()) {
                QMessageBox::warning(this, ui->leAsptSerNum->text(), tr("Не выбран ни один канал для измерения!"), tr("Хорошо"));
                ui->pbStartStop->setChecked(false);
                return;
            }
            MI::aspt()->ping(ui->cbxAspt->currentText());
            MI::upn()->ping(ui->cbxUpn->currentText());
            ui->pbStartStop->setIcon(Stop);
            ui->pbStartStop->setText(tr("Закончить проверку"));
            setProgressVisible(true);
            elapsedMs = 0;
            emit goMeasure(channels, ui->sbxMeasNum->value() + ui->sbxSkipMeasNum->value());
        } else {
            emit stopWork(3);
            ui->pbStartStop->setIcon(Start);
            ui->pbStartStop->setText(tr("Начать проверку"));
            progressBar.setValue(0);
            setProgressVisible(false);
        }
        ui->pbStartStop->setChecked(checked);
        ui->tabDevices->setEnabled(!checked);
        ui->tabUpn->setEnabled(!checked);
        ui->pbClear->setEnabled(!checked);
        ui->table->setEnabledCheckBoxes(!checked);
        menuBar()->setEnabled(!checked);
    });

    createMenus();

    QTimer::singleShot(100, [&]() {
        taskbarButton = new QWinTaskbarButton(this);
        taskbarButton->setWindow(windowHandle());
        taskbarProgress = taskbarButton->progress();
        //        connect(&progressBar, &QProgressBar::valueChanged, [&](int value) {
        //            taskbarProgress->setRange(0, progressBar.maximum());
        //            taskbarProgress->setValue(value);
        //        });
    });

    timeEdit.setButtonSymbols(QAbstractSpinBox::NoButtons);
    timeEdit.setDisplayFormat("H:mm:ss dd.MM.yyyyг.");
    timeEdit.setDateTime(QDateTime::currentDateTime());
    timeEdit.setReadOnly(true);
    ui->statusBar->addWidget(&timeEdit);

    progressBar.setValue(0);
    progressBar.setVisible(false);
    ui->statusBar->addWidget(&progressBar);
    lbRemainingTime.setVisible(false);
    lbRemainingTime.setText("Осталось: ?");
    ui->statusBar->addWidget(&lbRemainingTime);

    connect(&statusBarTimer, &QTimer::timeout, [&]() {
        timeEdit.setDateTime(QDateTime::currentDateTime());
        timeEdit.setCurrentSectionIndex(0);
    });

    statusBarTimer.start(1000);
}

void MainWindow::devFinderFinished()
{
    qDebug(__FUNCTION__);
    ui->pbFindDevices->setChecked(false);
    ui->pbFindDevices->setIcon(Start);
    ui->pbFindDevices->setText(tr("Поиск приборов."));
    ui->tabWidget->tabBar()->setEnabled(true);
    ui->tabUpn->setEnabled(ui->cbxUpn->count() > 0);

    if (ui->cbxUpn->count())
        MI::upn()->ping(ui->cbxUpn->currentText());
    if (ui->cbxAspt->count())
        MI::aspt()->ping(ui->cbxAspt->currentText());

    ui->pbStartStop->setEnabled(ui->cbxAspt->count() > 0); // && ui->cbxUpn->count() > 0);

    setProgressVal(0);
    finder = nullptr;
}

void MainWindow::handleMessage(eMessageType msgType, int row)
{
    int count = (ui->sbxMeasNum->value() + ui->sbxSkipMeasNum->value()) * 6;
    switch (msgType) {
    case ConnectUptToAspt:
        if (!curFile.isEmpty())
            saveFile(curFile);
        taskbarProgress->pause();
        ui->table->showRow(row);
        if (QMessageBox::information(this, ui->leAsptSerNum->text(), QString(tr("Воткни УПН №%1 в канал %2 АСПТ")).arg(ui->leUpnSerNum->text()).arg(row + 1), "Продолжить", "Остановить проверку", 0, 0) == 1) {
            emit stopWork();
        } else {
            ui->table->model()->clearData(row);
            progressBar.setMaximum(count);
            progressBar.setValue(0);
            taskbarProgress->resume();
            elapsedTimer.start();
        }
        emit stopWork();
        break;
    case CheckFinished:
        taskbarProgress->stop();
        activateWindow();
        QMessageBox::information(this, ui->leAsptSerNum->text(), tr("Проверка закончена!"), tr("Хорошо"), "");
        ui->pbStartStop->clicked(false);
        break;
    case TerminateCheck:
        ui->pbStartStop->clicked(false);
        break;
    case CheckUptConnection:
        ui->pbStartStop->clicked(false);
        QMessageBox::warning(this, ui->leAsptSerNum->text(), tr("Нет свяи с УПН!"), tr("Хорошо"), "");
        break;
    case CheckAsptConnection:
        ui->pbStartStop->clicked(false);
        QMessageBox::warning(this, ui->leAsptSerNum->text(), tr("Нет свяи с АСПТ!"), tr("Хорошо"), "");
        break;
    case CheckUptToAsptConnection:
        QMessageBox::warning(this, ui->leAsptSerNum->text(), tr("Проверь подключение УПН(%2) к каналу №%1 АСПТ!").arg(row + 1).arg(ui->leUpnSerNum->text()), tr("Хорошо"), "");
        emit stopWork();
        break;
    }
}

void MainWindow::handleMeasure(const double value, int ch, int r)
{
    progressBar.setValue(progressBar.value() + 1);
    elapsedMs += elapsedTimer.restart();
    int ms = 0;
    for (QPair<int, int>& var : channels) {
        if (var.first < ch)
            ++ms;
    }
    ms = (ms * progressBar.maximum()) + progressBar.value();
    ms = (elapsedMs / ms) * (progressBar.maximum() * channels.count() - ms);
    lbRemainingTime.setText("Осталось: " + QTime(0, 0).addMSecs(ms).toString("hh:mm:ss"));
    ui->table->model()->addData(ch, r, value);
}

void MainWindow::loadSettings()
{
    QSettings settings("AsptNonLinTest.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->leFio->setText(settings.value("leFio", "").toString());
    ui->sbxMeasNum->setValue(settings.value("sbxMeasNum", 5).toInt());
    ui->sbxSkipMeasNum->setValue(settings.value("sbxSkipMeasNum", 10).toInt());
    ui->dsbxMin->setValue(settings.value("dsbxMin", 0.0005).toDouble());
    ui->dsbxMax->setValue(settings.value("dsbxMax", 0.001).toDouble());
    //    MI::upn->setAdcCfg(settings.value("ADCCfg",
    //                                   QStringList({ "|1|1|1|0|0|0|3|6|28|3|0|0|5|0|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|220|0|\0",
    //                                       "|1|1|1|0|0|0|3|6|28|3|0|0|5|1|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|221|0|\0" }))
    //                           .toStringList());
    lastPath = settings.value("lastPath", qApp->applicationDirPath()).toString();
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings("AsptNonLinTest.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("leFio", ui->leFio->text());
    settings.setValue("sbxMeasNum", ui->sbxMeasNum->value());
    settings.setValue("sbxSkipMeasNum", ui->sbxSkipMeasNum->value());
    settings.setValue("dsbxMin", ui->dsbxMin->value());
    settings.setValue("dsbxMax", ui->dsbxMax->value());
    //    settings.setValue("ADCCfg", MI::upn->getAdcCfg());
    settings.setValue("lastPath", lastPath);
    settings.endGroup();
}

void MainWindow::createMenus()
{
    QAction* action;
    QMenu* menu;

    menu = menuBar()->addMenu(tr("&Файл"));
    action = menu->addAction(QIcon::fromTheme("document-open"), "&Открыть...", this, &MainWindow::onOpenAction, QKeySequence::Open);
    action->setStatusTip(tr("Открыть существующий файл"));

    action = menu->addAction(QIcon::fromTheme("document-save"), "&Сохранить...", this, &MainWindow::onSaveAction, QKeySequence::Save);
    action->setStatusTip(tr("Сохранить документ на диск"));

    action = menu->addAction(QIcon::fromTheme("document-save-as"), "Сохранить как...", this, &MainWindow::onSaveAsAction, QKeySequence::SaveAs);
    action->setStatusTip(tr("Сохраните документ под новым именем"));

    action = menu->addSeparator();

    menu->addAction(QIcon::fromTheme("document-print"), "Печать", this, &MainWindow::onPrintAction, QKeySequence::Print);
    action->setStatusTip(tr("Распечатать текущий документ"));

    menu->addSeparator();

    recentFiles.createMenu(menu, "Предыдущие протоколы");

    menu->addSeparator();

    action = menu->addAction(QIcon::fromTheme("window-close"), "Выход", qApp, &QApplication::closeAllWindows);
    action->setStatusTip(tr("Выход из приложения"));
    //    connect(fileMenu, &QMenu::aboutToShow, [this] { printAct->setEnabled(QFileInfo::exists(curFile)); });

    menu = menuBar()->addMenu(tr("&Справка"));
    action = menu->addAction(QIcon::fromTheme(""), "О &Qt", qApp, &QApplication::aboutQt);
    action->setStatusTip(tr("О библиотеке Qt"));
}

void MainWindow::openFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        QMessageBox::warning(this, tr("Недавние файлы"), tr("Не удается прочитать файл %1:\n%2.").arg(fileName).arg(file.errorString()), "TT");
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->table->openFile(fileName);
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    recentFiles.prependToRecentFiles(fileName);
    statusBar()->showMessage(tr("Файл загружен"), 2000);
}

void MainWindow::saveFile(const QString& fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->table->saveFile(fileName, ui->leAsptSerNum->text(), ui->leFio->text());
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл сохранен"), 2000);
}

void MainWindow::setCurrentFile(const QString& fileName)
{
    curFile = fileName;
    lastPath = QFileInfo(curFile).path();
    setWindowTitle(QFileInfo(curFile).fileName());
    //    recentFileActs.prependToRecentFiles(fileName);
}

QString MainWindow::strippedName(const QString& fullFileName) { return QFileInfo(fullFileName).fileName(); }

void MainWindow::onNewFileAction()
{
    MainWindow* other = new MainWindow;
    other->show();
}

void MainWindow::onOpenAction()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", lastPath, "Image Files (*.xls *.xlsx)");
    if (!fileName.isEmpty())
        openFile(fileName);
}

void MainWindow::onSaveAction()
{
    if (curFile.isEmpty())
        onSaveAsAction();
    else
        saveFile(curFile);
}

void MainWindow::onSaveAsAction()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", lastPath + "/" + ui->leAsptSerNum->text() + QDate::currentDate().toString(" от dd.MM.yyyy г.xlsx"), "(*.xls *.xlsx)");
    if (fileName.isEmpty())
        return;
    saveFile(fileName);
}

void MainWindow::onPrintAction()
{
    if (!curFile.isEmpty()) {
        ui->table->saveFile(curFile, ui->leAsptSerNum->text(), ui->leFio->text());
        ui->table->printFile(curFile);
    }
}

void MainWindow::onOpenRecentFileAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        openFile(action->data().toString());
}
