#include "mainwindow.h"
#include "measuring_interface/mi.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("XrSoft");
    QApplication::setApplicationName("AsptNonLinTest");

    QFont font;
    font.setPointSize(10);
    QApplication::setFont(font);

    QIcon::setThemeSearchPaths({ QCoreApplication::applicationDirPath() + "/icons/breeze/", QCoreApplication::applicationDirPath() + "/icons/breeze-dark/" });
    QIcon::setThemeName("breeze");

    MI mi;
    Q_UNUSED(mi)

    MainWindow w;
    w.show();
    return a.exec();
}
