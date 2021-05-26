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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    friend Recent;

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow* ui;
    QIcon Start;
    QIcon Stop;

    QTimer statusBarTimer;
    QProgressBar progressBar;
    QLabel lbRemainingTime;
    QDateTimeEdit timeEdit;

#ifndef linux
    QWinTaskbarButton* taskbarButton;
    QWinTaskbarProgress* taskbarProgress;
#endif

    //////////////////////////////////////////////////////////////////////////////////////
signals:
    void goFindDevices();
    void goMeasure(const QVector<QPair<int, int>>&, int);
    void stopWork(int count = 1);
    void setResistor(int r = 0);

private slots:
    //////////////////////////////////////////////////////////////////////////////////////
    void updatePlot(int chNum);
    void on_pbUpnRead_clicked();
    void on_pbUpnWrite_clicked();
    void setProgressVisible(bool fl);

    void currentIndexChanged(int index);

private:
    void connectObjects();
    //    void startStopMeasure(bool checked = false);
    void ResistorClicked();
    //////////////////////////////////////////////////////////////////////////////////////
    void handleDeviceFound(eDevice dev, const QString& portName, double num);
    void handleMessage(eMessageType i, int row);
    void handleMeasure(const double value, int ch, int r);
    //////////////////////////////////////////////////////////////////////////////////////
    void readSettings();
    void writeSettings();
    //////////////////////////////////////////////////////////////////////////////////////
    void tableWidgetCheckAll(bool checked);
    void tableWidgetCleaner();
    bool tableWidgetUpdated;
    //////////////////////////////////////////////////////////////////////////////////////
    void createActions();
    void createMenus();
    void loadFile(const QString& fileName);
    void saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);

    QString strippedName(const QString& fullFileName);

    QString curFile;
    QString lastPath;

    Recent recentFiles;

    QMenu* fileMenu;
    //    QMenu* recentFilesMenu;
    QMenu* helpMenu;
    //    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;
    QAction* aboutQtAct;
    QAction* printAct;

    QVector<QLineSeries*> series;

    QElapsedTimer elapsedTimer;
    int elapsedMs;
    QVector<QPair<int, int>> channels;

    //    enum { MaxRecentFiles = 5 };
    //    QAction* recentFileActs[MaxRecentFiles];
    //////////////////////////////////////////////////////////////////////////////////////
    //private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void print();
    void openRecentFile();
};


