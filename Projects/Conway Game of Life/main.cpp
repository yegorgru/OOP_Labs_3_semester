#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Game of Life");
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
