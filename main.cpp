#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("XrSoft");
    a.setApplicationName("AsptNonLinTest");
    QIcon::setThemeSearchPaths({ QCoreApplication::applicationDirPath() + "/icons/breeze/", QCoreApplication::applicationDirPath() + "/icons/breeze-dark/" });
    QIcon::setThemeName("breeze");
    //    QFile file(":/darcula.css");
    //    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //        QTextStream in(&file);
    //        a.setStyleSheet(in.readAll());
    //        file.close();
    //    }
    MainWindow w;
    w.show();
    return a.exec();
}
