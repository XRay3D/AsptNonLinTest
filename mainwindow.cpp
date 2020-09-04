#include "mainwindow.h"
#include "mytablemodel.h"
#include "ui_mainwindow.h"
#include <QChart>
#include <QLineSeries>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Start(QStringLiteral(":/res/image2.png") /*QIcon::fromTheme("draw-triangle2")*/)
    , Stop(QStringLiteral(":/res/image3.png") /*QIcon::fromTheme("draw-rectangle")*/)
    , recentFiles(this, "proto")
{
    ui->setupUi(this);

    auto setupChart = [&](QChartView* chartView, const QString& name) {
        QChart* chart = new QChart();
        chart->setTitle(name);
        QLineSeries* s;
        s = new QLineSeries(chart);
        s->setName("Подканал 0");
        chart->addSeries(s);
        series.append(s);
        s = new QLineSeries(chart);
        s->setName("Подканал 1");
        chart->addSeries(s);
        series.append(s);
        chart->legend()->setAlignment(Qt::AlignRight);
        chart->setMargins(QMargins(0, 0, 0, 0));
        chart->createDefaultAxes();
        chart->setTheme(QChart::ChartThemeDark);
        chartView->setChart(chart);
        chartView->setRenderHints(QPainter::Antialiasing);
        chartView->setBackgroundBrush(Qt::white);
    };
    setupChart(ui->graphicsView_1, "Сопротивление, Ом.\nR1");
    setupChart(ui->graphicsView_2, "Сопротивление, Ом.\nR2");
    setupChart(ui->graphicsView_3, "Сопротивление, Ом.\nR1+R2");

    connectObjects();
    readSettings();

    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    emit stopWork(3);
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void MainWindow::handleDeviceFound(eDevice dev, const QString& portName, double num)
{
    switch (dev) {
    case DeviceAspt:
        ui->cbxAspt->addItem(portName, int(num));
        break;
    case DeviceUpt:
        ui->cbxUpn->addItem(portName, QVariant::fromValue(MI::upn->resistors()));
        ui->tab_3->setEnabled(true);
        break;
    case DeviceProgres:
        progressBar.setValue(progressBar.value() + 1);
        break;
    case DeviceStopSearch:
        if (ui->cbxUpn->count())
            MI::upn->Ping(ui->cbxUpn->currentText());
        if (ui->cbxAspt->count())
            MI::aspt->Ping(ui->cbxAspt->currentText());

        if (ui->cbxAspt->count() && ui->cbxUpn->count())
            ui->pbStartStop->setEnabled(true);
        else
            ui->pbStartStop->setEnabled(false);
        ui->pbFindDevices->setChecked(false);
        ui->pbFindDevices->setIcon(Start);
        ui->pbFindDevices->setText(tr("Поиск приборов."));
        setProgressVisible(false);
    }
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

void MainWindow::updatePlot(int chNum)
{
    auto updateChart = [&](const QVector<int> n, QChartView* graphicsView) {
        QVector<double> y1(ui->table->model()->getData(chNum, n[0]));
        QVector<double> y2(ui->table->model()->getData(chNum, n[1]));
        series[n[2]]->clear();
        series[n[3]]->clear();
        if (y1.size() > 1 || y2.size() > 1) {
            for (int x = 0, end = y1.size(); x < end; ++x)
                series[n[2]]->append(x + 1, y1[x]);
            for (int x = 0, end = y2.size(); x < end; ++x)
                series[n[3]]->append(x + 1, y2[x]);
            graphicsView->chart()->axes(Qt::Horizontal).first()->setRange(1, qMax(y1.size(), y2.size())); /*axisX*/
            y1.append(y2);
            std::sort(y1.begin(), y1.end());
            graphicsView->chart()->axes(Qt::Vertical).first()->setRange(y1.first(), y1.last()); /*axisY*/
        } else {
            double d = 0.0;
            series[n[2]]->append(d, d);
            series[n[3]]->append(d, d);
        }
    };

    updateChart({ 0, 3, 0, 1 }, ui->graphicsView_1);
    updateChart({ 1, 4, 2, 3 }, ui->graphicsView_2);
    updateChart({ 2, 5, 4, 5 }, ui->graphicsView_3);
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::readSettings()
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

void MainWindow::writeSettings()
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

///////////////////////////////////////////////////////////////////////////////

void MainWindow::newFile()
{
    MainWindow* other = new MainWindow;
    other->show();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Открыть файл", lastPath, "Image Files (*.xls *.xlsx)");
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::save()
{
    if (curFile.isEmpty())
        saveAs();
    else
        saveFile(curFile);
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", lastPath + "/" + ui->leAsptSerNum->text() + QDate::currentDate().toString(" от dd.MM.yyyy г.xlsx"), "(*.xls *.xlsx)");
    if (fileName.isEmpty())
        return;
    saveFile(fileName);
}

void MainWindow::print()
{
    if (!curFile.isEmpty()) {
        ui->table->saveFile(curFile, ui->leAsptSerNum->text(), ui->leFio->text());
        ui->table->printFile(curFile);
    }
}

void MainWindow::openRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Открыть..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Открыть существующий файл"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Сохранить"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить документ на диск"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    saveAsAct = new QAction(tr("Сохранить как..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Сохраните документ под новым именем"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

    printAct = new QAction(tr("Печать"), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Распечатать текущий документ"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);

    exitAct = new QAction(tr("Выход"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Выход из приложения"));
    connect(exitAct, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    aboutQtAct = new QAction(tr("О &Qt"), this);
    aboutQtAct->setStatusTip(tr("Показать окно О библиотеке Qt"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    // fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);

    fileMenu->addSeparator();
    fileMenu->addAction(printAct);

    fileMenu->addSeparator();
    recentFiles.createMenu(fileMenu, "Предыдущие протоколы");

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpMenu->addAction(aboutQtAct);

    connect(fileMenu, &QMenu::aboutToShow, [this] { printAct->setEnabled(QFileInfo::exists(curFile)); });
}

void MainWindow::loadFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        QMessageBox::warning(this, tr("Недавние файлы"), tr("Не удается прочитать файл %1:\n%2.").arg(fileName).arg(file.errorString()), "TT");
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->table->loadFile(fileName);
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
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
    recentFiles.prependToRecentFiles(fileName);
}

QString MainWindow::strippedName(const QString& fullFileName) { return QFileInfo(fullFileName).fileName(); }

void MainWindow::on_pbUpnRead_clicked()
{
    MI::upn->Ping(ui->cbxUpn->currentText());
    if (MI::upn->readResistorValue()) {
        ui->dsbxUpnR1->setValue(MI::upn->resistors()[0]);
        ui->dsbxUpnR2->setValue(MI::upn->resistors()[1]);
        ui->dsbxUpnR3->setValue(MI::upn->resistors()[2]);
        ui->sbxUpnSerNum->setValue(static_cast<int>(MI::upn->resistors()[5]));
        QMessageBox::information(
            this, tr("УПН"), tr("Значения из УПН успешно считаны!"), "Хоошо :-)");
    } else {
        QMessageBox::critical(this, tr("УПН"), tr("Не удалось прочитать значения из УПН!"), "Плохо :-(");
    }
}

void MainWindow::on_pbUpnWrite_clicked()
{
    MI::upn->Ping(ui->cbxUpn->currentText());
    if (QMessageBox::question(this, tr("УПН"), tr("Вы действительно хотите записать новые значения в УПН?"), "Да", "Нет", "", 1, 1) == 0) {
        if (MI::upn->writeResistorValue(
                { ui->dsbxUpnR1->value(),
                    ui->dsbxUpnR2->value(),
                    ui->dsbxUpnR3->value(),
                    0.0,
                    0.0,
                    double(ui->sbxUpnSerNum->value()) })) {
            QMessageBox::information(this, tr("УПН"), tr("Новые значения в УПН успешно записаны!"), "Хоошо :-)");
            ui->cbxUpn->setItemData(ui->cbxUpn->currentIndex(), QVariant::fromValue(MI::upn->resistors()));
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
        ui->tab_3->setEnabled(true);
        //        ui->table->model()->setResistorsValue(ui->cbxUpn->currentData().value<QVector<double>>());
    } else {
        ui->leUpnSerNum->clear();
        ui->dsbxUpnR1->setValue(0.0);
        ui->dsbxUpnR2->setValue(0.0);
        ui->dsbxUpnR3->setValue(0.0);
        ui->sbxUpnSerNum->setValue(0);
        ui->tab_3->setEnabled(false);
    }
}

void MainWindow::connectObjects()
{
    /***************** Thread *****************/
    connect(this, &MainWindow::goMeasure, MI::measure, &Measure::measure);
    connect(this, &MainWindow::goFindDevices, MI::measure, &Measure::searchDevices);
    connect(this, &MainWindow::stopWork, MI::measure, &Measure::stopWork);

    connect(MI::measure, &Measure::deviceFound, this, &MainWindow::handleDeviceFound);
    connect(MI::measure, &Measure::measureReady, this, &MainWindow::handleMeasure);
    connect(MI::measure, &Measure::doMessage, this, &MainWindow::handleMessage);

    /***************** table->model() *****************/
    connect(ui->table, &MyTable::updatePlot, ui->cbxPlot, &QComboBox::setCurrentIndex);
    connect(ui->table, &MyTable::updatePlot, this, &MainWindow::updatePlot);
    connect(ui->cbxPlot, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::updatePlot);
    connect(ui->sbxSkipMeasNum, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->table->model(), &MyTableModel::setSkip);

    connect(ui->chbxDelta, &QCheckBox::toggled, ui->table, &MyTable::enableDelta);
    connect(ui->pbClear, &QPushButton::clicked, ui->table->model(), &MyTableModel::clearSelectedData);

    connect(ui->cbxAspt, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);
    connect(ui->cbxUpn, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);
    connect(ui->cbxUpn, qOverload<const QString&>(&QComboBox::currentIndexChanged), MI::upn, &Upn::Ping);

    connect(ui->pbFindDevices, &QPushButton::clicked, [=](bool checked) {
        if (checked) {
            //            disconnect(ui->cbxAspt, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);
            //            disconnect(ui->cbxUpn, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);
            ui->cbxAspt->clear();
            ui->cbxUpn->clear();
            ui->pbFindDevices->setIcon(Stop);
            ui->pbFindDevices->setText(tr("Остановить."));
            int count = QSerialPortInfo::availablePorts().count() * 2;
            progressBar.setMaximum(count);
            taskbarProgress->setRange(0, count);
            setProgressVisible(true);
            taskbarProgress->resume();
            progressBar.setValue(0);

            curFile.clear(); ////////////////////////

            emit goFindDevices();
        } else {
            emit stopWork();
        }
    });

    ui->tab_3->setEnabled(false);

    connect(this, &MainWindow::setResistor, MI::upn, &Upn::setResistor);
    connect(ui->pbUpnR1Ch0, &QPushButton::clicked, [this] { setResistor(0); });
    connect(ui->pbUpnR2Ch0, &QPushButton::clicked, [this] { setResistor(1); });
    connect(ui->pbUpnR3Ch0, &QPushButton::clicked, [this] { setResistor(2); });
    connect(ui->pbUpnR1Ch1, &QPushButton::clicked, [this] { setResistor(3); });
    connect(ui->pbUpnR2Ch1, &QPushButton::clicked, [this] { setResistor(4); });
    connect(ui->pbUpnR3Ch1, &QPushButton::clicked, [this] { setResistor(5); });

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
            MI::aspt->Ping(ui->cbxAspt->currentText());
            MI::upn->Ping(ui->cbxUpn->currentText());
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
        ui->tab_1->setEnabled(!checked);
        ui->tab_3->setEnabled(!checked);
        ui->pbClear->setEnabled(!checked);
        ui->table->setEnabledCheckBoxes(!checked);
        menuBar()->setEnabled(!checked);
    });

    createActions();
    createMenus();

    QTimer::singleShot(100, [&]() {
        taskbarButton = new QWinTaskbarButton(this);
        taskbarButton->setWindow(windowHandle());
        taskbarProgress = taskbarButton->progress();
        connect(&progressBar, &QProgressBar::valueChanged, [&](int value) {
            taskbarProgress->setRange(0, progressBar.maximum());
            taskbarProgress->setValue(value);
        });
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
