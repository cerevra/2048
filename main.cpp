#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("tr_ru", a.applicationDirPath());
    a.installTranslator(&translator);

    QApplication::setOrganizationName  ("Cerevra's soft");
    QApplication::setOrganizationDomain("unconditional.address");
    QApplication::setApplicationName   ("2048");

    MainWindow w;
    w.show();

    return a.exec();
}
