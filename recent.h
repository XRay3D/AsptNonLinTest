#pragma once

#include <QObject>

class QSettings;
class QAction;
class QMenu;

class Recent : public QObject {
    Q_OBJECT

public:
    explicit Recent(QObject* parent, QString&& recentFilesKey);

    enum { MaxRecentFiles = 20 };
    void createMenu(QMenu* fileMenu, const QString& menuName);

    QStringList readRecentFiles(QSettings& settings);
    bool hasRecentFiles();
    void openRecentFile();
    void prependToRecentFiles(const QString& fileName);
    void setRecentFilesVisible(bool visible);
    void updateRecentFileActions();
    void writeRecentFiles(const QStringList& files, QSettings& settings);

    QString fileKey();

private:
    const QString recentFilesKey;

    QMenu* recentMenu = nullptr;
    QAction* recentFileSeparator = nullptr;
    QAction* recentFileSubMenuAct = nullptr;
    QAction* recentFileActs[MaxRecentFiles + 1];
};
