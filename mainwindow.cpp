#include "mainwindow.h"
#include "measuremodel.h"
#include "measuring_interface/devfinder.h"
#include "measuring_interface/mi.h"

#include <QFileDialog>
#include <QMessageBox>

#if __has_include("myexcel.h")
#include "myexcel.h"
#define EXCEL 1
#endif

QString operator"" _qstr(const char* str, size_t) { return QString(str); }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , recentFiles(this, "proto")
    , model{new MeasureModel(this)} //
{
    m_instance = this;

    setupUi(this);
    tabCharts->setModel(model);
    tabMeasure->setModel(model);
    tabUpn->setUiWidgets(cbxUpn, leUpnSerNum);
    connectObjects();
    loadSettings();

    leAsptSerNum->setText("123456789");
    tabMeasure->setEnabledMeasure(true);

    tabWidget->setCurrentIndex(1);
    tabWidget->setTabIcon(0, QIcon::fromTheme("configure-shortcuts"));
    tabWidget->setTabIcon(1, QIcon::fromTheme("tool-measure"));
    tabWidget->setTabIcon(2, QIcon::fromTheme("document-edit"));
    tabWidget->setTabIcon(3, QIcon::fromTheme("office-chart-line"));

    tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() { }

void MainWindow::closeEvent(QCloseEvent* event) {
    saveSettings();
    if(tabMeasure->measure) {
        QMessageBox::warning(this, "", "Идёт проверка!");
        event->ignore();
    } else
        //    if(measure && QMessageBox::question(this, "", "Идёт проверка. Прекратить её?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        //        QEventLoop eventLoop;
        //        connect(measure, &QThread::finished, [&eventLoop] { eventLoop.exit(); });
        //        measure->requestInterruption();
        //        eventLoop.exec();
        //    }
        //    if(!curFile.isEmpty() && QMessageBox::question(this, "", "Сокранить протокол: %1"_qstr.arg(curFile), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        //        saveFile(curFile);
        event->accept();
}

void MainWindow::setProgressVisible(bool fl) {
    progressBar.setVisible(fl);
    lbRemainingTime.setVisible(fl);
    if(fl) {
        taskbarProgress->show();
        progressBar.setValue(0);
        lbRemainingTime.setText("Осталось: ?");
    } else
        taskbarProgress->hide();
}

void MainWindow::currentIndexChanged(int index) {
    index = cbxAspt->itemData(cbxAspt->currentIndex()).toInt();
    if(index > 0)
        leAsptSerNum->setText(QString("%1").arg(index).insert(2, '-'));
    else
        leAsptSerNum->clear();

    tabUpn->update();
}

void MainWindow::setProgressMax(int max) {
    progressBar.setMaximum(max);
    taskbarProgress->setRange(0, max);
}

void MainWindow::setProgressVal(int val) {
    if(val == 1) {
        setProgressVisible(true);
        elapsedTimer.restart();
    }
    if(!val) {
        setProgressVisible(false);
        elapsedMs = 0;
    }

    progressBar.setValue(val);
    taskbarProgress->resume();
    taskbarProgress->setValue(val);

    if(val) {
        elapsedMs += elapsedTimer.restart();
        int ms = (elapsedMs / progressBar.value());
        ms = ms * progressBar.maximum();
        ms = ms - elapsedMs;
        lbRemainingTime.setText("Осталось: " + QTime(0, 0).addMSecs(ms > 0 ? ms : 0).toString("hh:mm:ss"));
    }
}

void MainWindow::connectObjects() {
    tabUpn->setEnabled(false);

    connect(cbxAspt, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);
    connect(cbxUpn, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::currentIndexChanged);

    connect(cbxUpn, qOverload<const QString&>(&QComboBox::currentIndexChanged), [](const QString& portName) { MI::upn()->ping(portName); });

    connect(leAsptSerNum, &QLineEdit::textChanged, model, &MeasureModel::restore);

    connect(sbxSkipMeasNum, qOverload<int>(&QSpinBox::valueChanged), model, &MeasureModel::setSkip);

    connect(dsbxMin, qOverload<double>(&QDoubleSpinBox::valueChanged), [&](double arg1) {
        if(dsbxMax->value() < arg1)
            dsbxMax->setValue(arg1);
        model->setMin(arg1);
    });

    connect(dsbxMax, qOverload<double>(&QDoubleSpinBox::valueChanged), [&](double arg1) {
        if(dsbxMin->value() > arg1)
            dsbxMin->setValue(arg1);
        model->setMax(arg1);
    });

    connect(pbFindDevices, &QPushButton::clicked, this, &MainWindow::onPbFindDevicesClicked);

    createMenus();

    QTimer::singleShot(100, [this]() {
        taskbarButton = new QWinTaskbarButton(this);
        taskbarButton->setWindow(windowHandle());
        taskbarProgress = taskbarButton->progress();
    });

    timeEdit.setButtonSymbols(QAbstractSpinBox::NoButtons);
    timeEdit.setDateTime(QDateTime::currentDateTime());
    timeEdit.setDisplayFormat("H:mm:ss dd.MM.yyyyг.");
    timeEdit.setReadOnly(true);
    statusBar_->addWidget(&timeEdit);

    progressBar.setValue(0);
    progressBar.setVisible(false);
    statusBar_->addWidget(&progressBar);

    lbRemainingTime.setVisible(false);
    lbRemainingTime.setText("Осталось: ?");
    statusBar_->addWidget(&lbRemainingTime);

    connect(&statusBarTimer, &QTimer::timeout, [&]() {
        timeEdit.setDateTime(QDateTime::currentDateTime());
        timeEdit.setCurrentSectionIndex(0);
    });

    statusBarTimer.start(1000);
}
void MainWindow::onPbFindDevicesClicked(bool checked) {
    qDebug() << sender();
    if(checked) {
        finder = new DevFinder(cbxAspt, cbxUpn);
        connect(finder, &DevFinder::updateProgressVal, this, &MainWindow::setProgressVal);
        connect(finder, &DevFinder::updateProgressMax, this, &MainWindow::setProgressMax);
        connect(finder, &QThread::finished, finder, &QObject::deleteLater);
        connect(finder, &QThread::finished, this, &MainWindow::devFinderFinished);
        finder->start();
        pbFindDevices->setIcon(QIcon::fromTheme("media-playback-stop"));
        pbFindDevices->setText(tr("Остановить."));
        tabWidget->tabBar()->setEnabled(false);
    } else {
        if(sender() == finder) {
            pbFindDevices->setChecked(false);
            pbFindDevices->setIcon(QIcon::fromTheme("media-playback-start"));
            pbFindDevices->setText(tr("Поиск приборов."));
            tabWidget->tabBar()->setEnabled(true);

            tabUpn->setEnabled(cbxUpn->count() > 0);

            if(cbxUpn->count())
                MI::upn()->ping(cbxUpn->currentText());
            if(cbxAspt->count())
                MI::aspt()->ping(cbxAspt->currentText());

            tabMeasure->setEnabledMeasure(cbxAspt->count() > 0); // && ui->cbxUpn->count() > 0);

            setProgressVal(0);
            finder = nullptr;
        }
        if(sender() == pbFindDevices) {
            if(eventLoop.isRunning())
                return;
            connect(finder, &QThread::finished, [this] { eventLoop.exit(); });
            finder->requestInterruption();
            eventLoop.exec();
        }
    }
}

void MainWindow::devFinderFinished() { onPbFindDevicesClicked(); }

void MainWindow::loadSettings() {
    QSettings settings("AsptNonLinTest.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    leFio->setText(settings.value("leFio", "").toString());
    sbxMeasNum->setValue(settings.value("sbxMeasNum", 5).toInt());
    sbxSkipMeasNum->setValue(settings.value("sbxSkipMeasNum", 10).toInt());
    dsbxMin->setValue(settings.value("dsbxMin", 0.0005).toDouble());
    dsbxMax->setValue(settings.value("dsbxMax", 0.001).toDouble());
    lastPath = settings.value("lastPath", qApp->applicationDirPath()).toString();
    settings.endGroup();
}

void MainWindow::saveSettings() {
    QSettings settings("AsptNonLinTest.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("leFio", leFio->text());
    settings.setValue("sbxMeasNum", sbxMeasNum->value());
    settings.setValue("sbxSkipMeasNum", sbxSkipMeasNum->value());
    settings.setValue("dsbxMin", dsbxMin->value());
    settings.setValue("dsbxMax", dsbxMax->value());
    settings.setValue("lastPath", lastPath);
    settings.endGroup();
}

void MainWindow::createMenus() {
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
    action = menu->addAction(QIcon::fromTheme("help-about"), "О &Qt", qApp, &QApplication::aboutQt);
    action->setStatusTip(tr("О библиотеке Qt"));
}

void MainWindow::openFile(const QString& fileName) {
    QFile file(fileName);
    if(!file.exists()) {
        QMessageBox::warning(this, tr("Недавние файлы"), tr("Не удается прочитать файл %1:\n%2.").arg(fileName).arg(file.errorString()), "TT");
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // FIXME   tableView->openFile(fileName);
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
#ifdef EXCEL
    MyExcel().loadFile(fileName, model);
#endif
    statusBar()->showMessage(tr("Файл загружен"), 2000);
}

void MainWindow::saveFile(const QString& fileName) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();
    setCurrentFile(fileName);
#ifdef EXCEL
    MyExcel().saveFile(fileName, leAsptSerNum->text(), leFio->text(), model);
#endif
    statusBar()->showMessage(tr("Файл сохранен"), 2000);
}

void MainWindow::setCurrentFile(const QString& fileName) {
    curFile = fileName;
    lastPath = QFileInfo(curFile).path();
    recentFiles.prependToRecentFiles(fileName);
    setWindowTitle(strippedName(curFile));
}

QString MainWindow::strippedName(const QString& fullFileName) { return QFileInfo(fullFileName).fileName(); }

void MainWindow::onNewFileAction() {
    MainWindow* other = new MainWindow;
    other->show();
}

void MainWindow::onOpenAction() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", lastPath, "Image Files (*.xls *.xlsx)");
    if(!fileName.isEmpty())
        openFile(fileName);
}

void MainWindow::onSaveAction() {
    if(curFile.isEmpty())
        onSaveAsAction();
    else
        saveFile(curFile);
}

void MainWindow::onSaveAsAction() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", lastPath + "/" + leAsptSerNum->text() + QDate::currentDate().toString(" от dd.MM.yyyy г.xlsx"), "(*.xls *.xlsx)");
    if(fileName.isEmpty())
        return;
    saveFile(fileName);
}

void MainWindow::onPrintAction() {
    if(!curFile.isEmpty()) {
#ifdef EXCEL
        MyExcel().saveFile(curFile, leAsptSerNum->text(), leFio->text(), model);
        MyExcel().printFile(curFile);
#endif
    }
}

void MainWindow::onOpenRecentFileAction() {
    QAction* action = qobject_cast<QAction*>(sender());
    if(action)
        openFile(action->data().toString());
}
