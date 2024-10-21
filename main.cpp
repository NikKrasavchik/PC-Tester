#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Recources/App_Logo.png"));
    MainWindow w;
    w.show();
    return a.exec();
}
