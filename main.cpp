#include <QApplication>

#include "WebView/CalendarView.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    CalendarView view;
    return app.exec();
}