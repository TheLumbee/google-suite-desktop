#ifndef CALENDARVIEW_HPP
#define CALENDARVIEW_HPP

#include "mainwindow.hpp"

#include <QSystemTrayIcon>
#include <QWebEngineView>

class CalendarView : public QWebEngineView
{
    Q_OBJECT

public:
    CalendarView(QWidget* parent = nullptr);
    ~CalendarView();
    bool IsShownOnStartup() { return m_showOnStartup; }

private:
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    bool m_showOnStartup;
    void SetupTrayIcon();
    void InitializeSettings();
    MainWindow* m_mainWindow = nullptr;

signals:
    void CalendarClosed();
    void CalendarOpened();
};
#endif // CALENDARVIEW_HPP