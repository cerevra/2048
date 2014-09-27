#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName  ("Cerevra's soft");
    QApplication::setOrganizationDomain("unconditional.address");
    QApplication::setApplicationName   ("2048");

    MainWindow w;
    w.show();

    return a.exec();
}
