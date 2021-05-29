#include "mainwindow.h"
#include "measuring_interface/mi.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QApplication::setOrganizationName("XrSoft");
    QApplication::setApplicationName("AsptNonLinTest");

    QFont font;
    font.setPointSize(10);
    QApplication::setFont(font);

    MI mi;
    Q_UNUSED(mi)

    //    QFile file(":/darcula.css");
    //    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //        QTextStream in(&file);
    //        a.setStyleSheet(in.readAll());
    //        file.close();
    //    }

    QIcon::setThemeName("breeze_xr");

    MainWindow w;
    w.show();

    return a.exec();
}
