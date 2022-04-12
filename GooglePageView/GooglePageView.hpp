#ifndef GOOGLEPAGEVIEW_HPP
#define GOOGLEPAGEVIEW_HPP

#include "../mainwindow.hpp"

#include <QSystemTrayIcon>
#include <QtWebEngineWidgets/qwebengineview.h>

class QWebEngineNotification;

class GooglePageView : public QWebEngineView
{
    Q_OBJECT

public:
    GooglePageView(QWidget* parent = nullptr);
    bool showOnStartup;

private:
    void InitializeSettings();
    MainWindow *mainWindow = nullptr;
    QList<QWebEnginePage*> webPages;

signals:
    void PageClosed();
    void PageOpened();
};
#endif // CALENDARVIEW_HPP