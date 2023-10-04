#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QTranslator myappTranslator;
    myappTranslator.load("translations/my_ru.qm");
    a.installTranslator(&myappTranslator);

    QTranslator defaultTranslator;
    defaultTranslator.load("translations/qt_ru.qm");
    a.installTranslator(&defaultTranslator);

    auto w = MainWindow::createClient();
    w->show();

    return a.exec();
}
