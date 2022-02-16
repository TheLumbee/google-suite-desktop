#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1200, 742);
    return app.exec();
}