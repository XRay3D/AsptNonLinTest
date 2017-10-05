#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Start(QStringLiteral(":/res/image2.png") /*QIcon::fromTheme("draw-triangle2")*/)
    , Stop(QStringLiteral(":/res/image3.png") /*QIcon::fromTheme("draw-rectangle")*/)
    , asptUpn(new AsptUpn)
{
    ui->setupUi(this);
    setupPlot(ui->plot_1, "Сопротивление, Ом.\nR1");
    setupPlot(ui->plot_2, "Сопротивление, Ом.\nR2");
    setupPlot(ui->plot_3, "Сопротивление, Ом.\nR1+R2");
    connectObjects();
    readSettings();
}

MainWindow::~MainWindow()
{
    emit stopWork();
    emit stopWork();
    emit stopWork();
    Thread.quit();
    Thread.wait();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void MainWindow::handleDeviceFound(Device dev, const QString& portName, double num)
{
    switch (dev) {
    case DeviceAspt:
        ui->cbxAspt->addItem(portName, int(num));
        break;
    case DeviceUpt:
        ui->cbxUpn->addItem(portName, QVariant::fromValue(asptUpn->getResistos()));
        ui->tab_3->setEnabled(true);
        break;
    case DeviceProgres:
        progressBar.setValue(progressBar.value() + 1);
    case DeviceStopSearch:
        if (progressBar.value() == progressBar.maximum() || dev == DeviceStopSearch) {
            asptUpn->setPortName(ui->cbxUpn->currentText());
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
}

void MainWindow::handleMessage(MessageType msgType, int row)
{
    int count = (ui->sbxMeasNum->value() + ui->sbxSkipMeasNum->value()) * 6;
    switch (msgType) {
    case ConnectUptToAspt:
        taskbarProgress->pause();
        ui->table->scrollToItem(ui->table->item(row, 0));
        if (QMessageBox::information(this, messageTitle, QString(tr("Воткни УПН №%1 в канал %2 АСПТ")).arg(ui->leUpnSerNum->text()).arg(row + 1), "Продолжить", "Остановить проверку", 0, 0) == 1) {
            emit stopWork();
        }
        else {
            ui->table->clearData(row);
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
        QMessageBox::information(this, messageTitle, tr("Проверка закончена!"), tr("Хорошо"), "");
        ui->pbStartStop->clicked(false);
        break;
    case TerminateCheck:
        ui->pbStartStop->clicked(false);
        break;
    case CheckUptConnection:
        ui->pbStartStop->clicked(false);
        QMessageBox::warning(this, messageTitle, tr("Нет свяи с УПТ!"), tr("Хорошо"), "");
        break;
    case CheckAsptConnection:
        ui->pbStartStop->clicked(false);
        QMessageBox::warning(this, messageTitle, tr("Нет свяи с АСПТ!"), tr("Хорошо"), "");
        break;
    case CheckUptToAsptConnection:
        QMessageBox::warning(this, messageTitle, tr("Проверь подключение УПТ(%2) к каналу №%1 АСПТ!").arg(row + 1).arg(ui->leUpnSerNum->text()), tr("Хорошо"), "");
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
    ui->table->addData(ch, r, value);
}

void MainWindow::updatePlot(int chNum)
{
    ui->plot_1->graph(0)->setData(ui->table->getData(chNum, 0));
    ui->plot_2->graph(0)->setData(ui->table->getData(chNum, 1));
    ui->plot_3->graph(0)->setData(ui->table->getData(chNum, 2));
    ui->plot_1->graph(1)->setData(ui->table->getData(chNum, 3));
    ui->plot_2->graph(1)->setData(ui->table->getData(chNum, 4));
    ui->plot_3->graph(1)->setData(ui->table->getData(chNum, 5));

    ui->plot_1->graph(0)->setVisible(ui->plot_1->graph(0)->data()->count() != 0);
    ui->plot_1->graph(1)->setVisible(ui->plot_1->graph(1)->data()->count() != 0);
    ui->plot_2->graph(0)->setVisible(ui->plot_2->graph(0)->data()->count() != 0);
    ui->plot_2->graph(1)->setVisible(ui->plot_2->graph(1)->data()->count() != 0);
    ui->plot_3->graph(0)->setVisible(ui->plot_3->graph(0)->data()->count() != 0);
    ui->plot_3->graph(1)->setVisible(ui->plot_3->graph(1)->data()->count() != 0);

    ui->plot_1->xAxis->rescale(true);
    ui->plot_1->yAxis->rescale(true);
    ui->plot_2->xAxis->rescale(true);
    ui->plot_2->yAxis->rescale(true);
    ui->plot_3->xAxis->rescale(true);
    ui->plot_3->yAxis->rescale(true);

    ui->plot_1->replot();
    ui->plot_2->replot();
    ui->plot_3->replot();
}

void MainWindow::setupPlot(QCustomPlot* plot, const QString& label)
{
    QCPGraph* graph;
    graph = plot->addGraph();
    graph->setName(tr("Подканал 0"));
    graph->setPen(QPen(Qt::red));
    graph->setScatterStyle(QCPScatterStyle::ssDisc);

    graph = plot->addGraph();
    graph->setName(tr("Подканал 1"));
    graph->setPen(QPen(Qt::blue));
    graph->setScatterStyle(QCPScatterStyle::ssDisc);

    plot->legend->setVisible(true);
    plot->legend->setMargins(QMargins(-20, -20, -20, -20));
    plot->legend->setRowSpacing(-10);
    plot->yAxis->setLabel(label);
    plot->yAxis->setNumberFormat("f");
    plot->yAxis->setNumberPrecision(5);
    plot->xAxis->setAutoTickStep(true);
    //plot->xAxis->setNumberPrecision(0);

    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(
        plot->xAxis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged),
        plot->xAxis2, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::setRange));
    connect(
        plot->yAxis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged),
        plot->yAxis2, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::setRange));
    plot->replot();
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->leFio->setText(settings.value("leFio", "").toString());
    ui->sbxMeasNum->setValue(settings.value("sbxMeasNum", 5).toInt());
    ui->sbxSkipMeasNum->setValue(settings.value("sbxSkipMeasNum", 10).toInt());
    ui->dsbxMin->setValue(settings.value("dsbxMin", 0.0005).toDouble());
    ui->dsbxMax->setValue(settings.value("dsbxMax", 0.001).toDouble());
    asptUpn->setAdcCfg(settings.value("ADCCfg",
                                   QStringList({ "|1|1|1|0|0|0|3|6|28|3|0|0|5|0|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|220|0|\0",
                                       "|1|1|1|0|0|0|3|6|28|3|0|0|5|1|20|6|1|28|3|0|0|5|6|20|6|1|28|3|0|0|5|5|20|6|1|0|0|0|0|5|0|1|1|1|0|0|0|0|0|0|1|1|1|0|0|0|0|0|0|1|1|1|221|0|\0" }))
                           .toStringList());
    lastPath = settings.value("lastPath", qApp->applicationDirPath()).toString();
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("leFio", ui->leFio->text());
    settings.setValue("sbxMeasNum", ui->sbxMeasNum->value());
    settings.setValue("sbxSkipMeasNum", ui->sbxSkipMeasNum->value());
    settings.setValue("dsbxMin", ui->dsbxMin->value());
    settings.setValue("dsbxMax", ui->dsbxMax->value());
    settings.setValue("ADCCfg", asptUpn->getAdcCfg());
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
        this, "Открыть файл", lastPath, "Image Files (*.xls *.xlsx *.csv)");
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
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", lastPath + "/" + messageTitle + QDate::currentDate().toString(" от dd.MM.yyyy г.xlsx"), "(*.xls *.xlsx)");
    if (fileName.isEmpty())
        return;
    saveFile(fileName);
}

void MainWindow::print()
{
    if (!curFile.isEmpty()) {
        ui->table->SaveFile(curFile, messageTitle, ui->leFio->text());
        ui->table->PrintFile(curFile);
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

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], &QAction::triggered, this, &MainWindow::openRecentFile);
    }

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
    separatorAct = fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    updateRecentFileActions();

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::loadFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.exists()) {
        QMessageBox::warning(this, tr("Недавние файлы"), tr("Не удается прочитать файл %1:\n%2.").arg(fileName).arg(file.errorString()), "TT");
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->table->LoadFile(fileName);
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл загружен"), 2000);
}

void MainWindow::saveFile(const QString& fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->table->SaveFile(fileName, messageTitle, ui->leFio->text());
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл сохранен"), 2000);
}

void MainWindow::setCurrentFile(const QString& fileName)
{
    curFile = fileName;
    lastPath = QFileInfo(curFile).path();
    setWindowTitle(QFileInfo(curFile).fileName());
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);

    foreach (QWidget* widget, QApplication::topLevelWidgets()) {
        MainWindow* mainWin = qobject_cast<MainWindow*>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::on_pbUpnRead_clicked()
{
    asptUpn->setPortName(ui->cbxUpn->currentText());
    if (asptUpn->readResistorValue()) {
        ui->dsbxUpnR1->setValue(asptUpn->getResistos()[0]);
        ui->dsbxUpnR2->setValue(asptUpn->getResistos()[1]);
        ui->dsbxUpnR3->setValue(asptUpn->getResistos()[2]);
        ui->sbxUpnSerNum->setValue(asptUpn->getResistos()[5]);
        QMessageBox::information(
            this, tr("УПН"), tr("Значения из УПН успешно считаны!"), "Хоошо :-)");
    }
    else {
        QMessageBox::critical(this, tr("УПН"), tr("Не удалось прочитать значения из УПН!"), "Плохо :-(");
    }
}

void MainWindow::on_pbUpnWrite_clicked()
{
    asptUpn->setPortName(ui->cbxUpn->currentText());
    if (QMessageBox::question(this, tr("УПН"), tr("Вы действительно хотите записать новые значения в УПН?"), "Да", "Нет", "", 1, 1) == 0) {
        asptUpn->getResistos() = {
            ui->dsbxUpnR1->value(),
            ui->dsbxUpnR2->value(),
            ui->dsbxUpnR3->value(),
            0.0,
            0.0,
            double(ui->sbxUpnSerNum->value())
        };

        if (asptUpn->writeResistorValue()) {
            QMessageBox::information(this, tr("УПН"), tr("Новые значения в УПН успешно записаны!"), "Хоошо :-)");
            ui->cbxUpn->setItemData(ui->cbxUpn->currentIndex(), QVariant::fromValue(asptUpn->getResistos()));
            ui->table->setResistors(ui->cbxUpn->currentData().value<QVector<double> >());
            ui->leUpnSerNum->setText(ui->sbxUpnSerNum->text());
        }
        else {
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
    }
    else
        taskbarProgress->hide();
}

void MainWindow::connectObjects()
{
    /***************** Thread *****************/
    asptUpn->moveToThread(&Thread);
    connect(&Thread, &QThread::finished, asptUpn, &QObject::deleteLater);
    connect(this, &MainWindow::goMeasure, asptUpn, &AsptUpn::measure);
    connect(this, &MainWindow::goFindDevices, asptUpn, &AsptUpn::searchDevices);
    connect(this, &MainWindow::stopWork, /*&Thread, &QThread::requestInterruption); //*/ asptUpn, &AsptUpn::stopWork);
    connect(this, &MainWindow::setResistor, asptUpn, &AsptUpn::setResistor);
    connect(asptUpn, &AsptUpn::deviceFound, this, &MainWindow::handleDeviceFound);
    connect(asptUpn, &AsptUpn::measureReady, this, &MainWindow::handleMeasure);
    connect(asptUpn, &AsptUpn::doMessage, this, &MainWindow::handleMessage);
    Thread.start();

    /***************** table *****************/
    connect(ui->table, &TABLE::updatePlot, ui->cbxPlot, &QComboBox::setCurrentIndex);
    connect(ui->table, &TABLE::updatePlot, this, &MainWindow::updatePlot);
    connect(ui->cbxPlot, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::updatePlot);
    connect(ui->sbxSkipMeasNum, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->table, &TABLE::setSkip);

    connect(ui->checkBox_setResetAll, &QCheckBox::toggled, ui->table, &TABLE::CheckUncheckAll);
    connect(ui->checkBox_ppm, &QCheckBox::toggled, ui->table, &TABLE::CheckPpm);
    connect(ui->checkBox_delta, &QCheckBox::toggled, ui->table, &TABLE::CheckDelta);
    connect(ui->pbClear, &QPushButton::clicked, ui->table, &TABLE::clearSelectedData);
    connect(ui->leAsptSerNum, &QLineEdit::textChanged, [&](const QString& text) { messageTitle = text; });

    auto currentIndexChanged = [=](int index) {
        index = ui->cbxAspt->itemData(ui->cbxAspt->currentIndex()).toInt();
        if (index > 0)
            ui->leAsptSerNum->setText(QString("%1").arg(index).insert(2, '-'));
        else
            ui->leAsptSerNum->clear();
        QVector<double> r(ui->cbxUpn->currentData().value<QVector<double> >());
        if (r.size() == 6) {
            ui->leUpnSerNum->setText(QString("%1").arg(r[5]));
            ui->dsbxUpnR1->setValue(r[0]);
            ui->dsbxUpnR2->setValue(r[1]);
            ui->dsbxUpnR3->setValue(r[2]);
            ui->sbxUpnSerNum->setValue(r[5]);
            ui->tab_3->setEnabled(true);
            ui->table->setResistors(ui->cbxUpn->currentData().value<QVector<double> >());
        }
        else {
            ui->leUpnSerNum->clear();
            ui->dsbxUpnR1->setValue(0.0);
            ui->dsbxUpnR2->setValue(0.0);
            ui->dsbxUpnR3->setValue(0.0);
            ui->sbxUpnSerNum->setValue(0);
            ui->tab_3->setEnabled(false);
        }
    };
    connect(ui->cbxAspt, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), currentIndexChanged);
    connect(ui->cbxUpn, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), currentIndexChanged);
    connect(ui->cbxUpn, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), asptUpn, &AsptUpn::setPortName);

    connect(ui->pbFindDevices, &QPushButton::clicked, [=](bool checked) {
        if (checked) {
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
            emit goFindDevices();
        }
        else {
            emit stopWork();
        }
    });

    ui->tab_3->setEnabled(false);

    connect(ui->pbUpnR1Ch0, &QPushButton::clicked, [this]() { emit setResistor(0); });
    connect(ui->pbUpnR2Ch0, &QPushButton::clicked, [this]() { emit setResistor(1); });
    connect(ui->pbUpnR3Ch0, &QPushButton::clicked, [this]() { emit setResistor(2); });
    connect(ui->pbUpnR1Ch1, &QPushButton::clicked, [this]() { emit setResistor(3); });
    connect(ui->pbUpnR2Ch1, &QPushButton::clicked, [this]() { emit setResistor(4); });
    connect(ui->pbUpnR3Ch1, &QPushButton::clicked, [this]() { emit setResistor(5); });

    connect(ui->dsbxMin, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double arg1) {
        if (ui->dsbxMax->value() < arg1)
            ui->dsbxMax->setValue(arg1);
        ui->table->setMin(arg1);
    });

    connect(ui->dsbxMax, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double arg1) {
        if (ui->dsbxMin->value() > arg1)
            ui->dsbxMin->setValue(arg1);
        ui->table->setMax(arg1);
    });

    connect(ui->pbStartStop, &QPushButton::clicked, [=](bool checked) {
        if (checked) {
            channels.clear();
            for (int row = 0; row < 16; ++row) {
                if (ui->table->item(row, 0)->checkState() == Qt::Checked)
                    channels.append(qMakePair(int(row), int(3)));
            }
            if (channels.isEmpty()) {
                QMessageBox::warning(this, messageTitle, tr("Не выбран ни один канал для измерения!"), tr("Хорошо"));
                ui->pbStartStop->setChecked(false);
                return;
            }
            ui->table->setEnableRow(false);
            asptUpn->setPortName(ui->cbxUpn->currentText());
            asptUpn->DevSetTieParameters(ui->cbxAspt->currentText(), 9600, 1);
            ui->pbStartStop->setIcon(Stop);
            ui->pbStartStop->setText(tr("Закончить проверку"));
            ui->table->setCurFile(curFile);
            setProgressVisible(true);
            elapsedMs = 0;
            emit goMeasure(channels, ui->sbxMeasNum->value() + ui->sbxSkipMeasNum->value());
        }
        else {
            emit stopWork();
            emit stopWork();
            emit stopWork();
            ui->table->setEnableRow(true);
            ui->pbStartStop->setIcon(Start);
            ui->pbStartStop->setText(tr("Начать проверку"));
            progressBar.setValue(0);
            setProgressVisible(false);
        }
        ui->pbStartStop->setChecked(checked);
        ui->tab_1->setEnabled(!checked);
        ui->tab_3->setEnabled(!checked);
        ui->pbClear->setEnabled(!checked);
        ui->checkBox_setResetAll->setEnabled(!checked);
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
