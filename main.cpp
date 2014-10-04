#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    translator.load("tr_ru", app.applicationDirPath());
    app.installTranslator(&translator);

    QApplication::setOrganizationName  ("Cerevra's soft");
    QApplication::setOrganizationDomain("unconditional.address");
    QApplication::setApplicationName   ("2048");

    MainWindow window;
    window.show();

    return app.exec();
}
