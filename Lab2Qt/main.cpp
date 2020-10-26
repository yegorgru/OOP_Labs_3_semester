#include "mainwindow.h"

#include <QApplication>

//https://stackoverflow.com/questions/3137805/how-to-set-image-on-qpushbutton

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
