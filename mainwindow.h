#pragma once

#include <QDateTimeEdit>
#include <QEvent>
#include <QLabel>
#include <QLineSeries>
#include <QList>
#include <QMainWindow>
#include <QProgressBar>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QtWinExtras>

#include "measuring_interface/mi.h"
#include "mytable.h"
#include "recent.h"

QT_CHARTS_USE_NAMESPACE

class QAction;
class QMenu;
class QTableWidgetItem;
class QCustomPlot;

class DevFinder;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    friend class Recent;

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void updatePlot(int chNum);
    void on_pbUpnRead_clicked();
    void on_pbUpnWrite_clicked();
    void setProgressVisible(bool fl);
    void currentIndexChanged(int index);

signals:
    void goMeasure(const QVector<QPair<int, int>>&, int);
    void stopWork(int count = 1);
    void setResistor(int r = 0);

private:
    Ui::MainWindow* ui;

#ifndef linux
    QWinTaskbarButton* taskbarButton;
    QWinTaskbarProgress* taskbarProgress;
#endif

    QIcon Start;
    QIcon Stop;

    Recent recentFiles;

    QDateTimeEdit timeEdit;
    QElapsedTimer elapsedTimer;
    QLabel lbRemainingTime;
    QProgressBar progressBar;
    QString curFile;
    QString lastPath;
    QString messageTitle;
    QTimer statusBarTimer;
    QVector<QLineSeries*> series;
    QVector<QPair<int, int>> channels;
    DevFinder* finder = nullptr;

    void setProgressMax(int max);
    void setProgressVal(int val);

    int elapsedMs;

    void connectObjects();
    void resistorClicked();
    // handles

    void devFinderFinished();
    void handleMessage(eMessageType msgType, int row);
    void handleMeasure(const double value, int ch, int r);
    // settings
    void loadSettings();
    void saveSettings();
    // table
    void tableWidgetCheckAll(bool checked);
    void tableWidgetCleaner();
    bool tableWidgetUpdated;
    // File
    void createMenus();

    void openFile(const QString& fileName);
    void saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);
    void updateRecentFileActions();

    QString strippedName(const QString& fullFileName);

    void onNewFileAction();
    void onOpenAction();
    void onSaveAction();
    void onSaveAsAction();
    void onPrintAction();
    void onOpenRecentFileAction();
};
