#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTimeEdit>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QProgressBar>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QtWinExtras>
#include <QEvent>

#include "aspt_upn.h"
#include "table.h"

class QAction;
class QMenu;
class QTableWidgetItem;
class QCustomPlot;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindow* ui;
    QIcon Start;
    QIcon Stop;
    AsptUpn* asptUpn;
    QThread Thread;


    QString messageTitle;

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
    void goMeasure(const QVector<QPair<int, int> >&, int);
    void stopWork();
    void setResistor(int);

private slots:
    //////////////////////////////////////////////////////////////////////////////////////
    void updatePlot(int chNum);
    void setupPlot(QCustomPlot* plot, const QString& label);
    void on_pbUpnRead_clicked();
    void on_pbUpnWrite_clicked();
    void setProgressVisible(bool fl);

private:
    void connectObjects();
    //    void startStopMeasure(bool checked = false);
    void ResistorClicked();
    //////////////////////////////////////////////////////////////////////////////////////
    void handleDeviceFound(Device dev, const QString& portName, double num);
    void handleMessage(MessageType i, int row);
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
    void updateRecentFileActions();

    QString strippedName(const QString& fullFileName);

    QString curFile;
    QString lastPath;

    QMenu* fileMenu;
    QMenu* recentFilesMenu;
    QMenu* helpMenu;
    //    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;
    QAction* aboutQtAct;
    QAction* separatorAct;
    QAction* printAct;

    QElapsedTimer elapsedTimer;
    int elapsedMs;
    QVector<QPair<int, int> > channels;

    enum { MaxRecentFiles = 5 };
    QAction* recentFileActs[MaxRecentFiles];
    //////////////////////////////////////////////////////////////////////////////////////
    //private slots:
    void newFile();
    void open();
    void save();
    void saveAs();
    void print();
    void openRecentFile();
};

#endif // MAINWINDOW_H
