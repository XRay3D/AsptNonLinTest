#pragma once

#include "recent.h"
#include "ui_mainwindow.h"

#include <QDateTimeEdit>
#include <QProgressBar>
#include <QtWinExtras>

class DevFinder;

class MainWindow : public QMainWindow, public Ui::MainWindow {
    Q_OBJECT
    friend class Recent;
    friend class TabMeasure;
    static inline MainWindow* m_instance;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    static MainWindow* instance() { return m_instance; }

protected:
    void closeEvent(QCloseEvent* event);

private:
#ifndef linux
    QWinTaskbarButton* taskbarButton;
    QWinTaskbarProgress* taskbarProgress;
#endif

    Recent recentFiles;

    DevFinder* finder = nullptr;
    MeasureModel* model = nullptr;
    QDateTimeEdit timeEdit;
    QElapsedTimer elapsedTimer;
    QEventLoop eventLoop;
    QLabel lbRemainingTime;
    QProgressBar progressBar;
    QString curFile;
    QString lastPath;
    QTimer statusBarTimer;
    int elapsedMs{};

    void setProgressMax(int max);
    void setProgressVal(int val);

    void setProgressVisible(bool fl);
    void currentIndexChanged(int index);

    void connectObjects();

    void onPbFindDevicesClicked(bool checked = {});

    // handles
    void devFinderFinished();

    // settings
    void loadSettings();
    void saveSettings();

    // File
    void createMenus();

    void openFile(const QString& fileName);
    void saveFile(const QString& fileName);
    void setCurrentFile(const QString& fileName);

    QString strippedName(const QString& fullFileName);

    void onNewFileAction();
    void onOpenAction();
    void onSaveAction();
    void onSaveAsAction();
    void onPrintAction();
    void onOpenRecentFileAction();
};
