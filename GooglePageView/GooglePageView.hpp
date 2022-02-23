#ifndef CALENDARVIEW_HPP
#define CALENDARVIEW_HPP

#include "mainwindow.hpp"

#include <QSystemTrayIcon>
#include <QWebEngineView>

class GooglePageView : public QWebEngineView
{
    Q_OBJECT

public:
    GooglePageView(QWidget* parent = nullptr);
    bool m_showOnStartup;

private:
    void InitializeSettings();
    MainWindow* m_mainWindow = nullptr;

signals:
    void PageClosed();
    void PageOpened();
};
#endif // CALENDARVIEW_HPP