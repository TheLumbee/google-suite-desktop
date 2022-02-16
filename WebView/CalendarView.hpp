#ifndef CALENDARVIEW_HPP
#define CALENDARVIEW_HPP

#include <QSettings>
#include <QSystemTrayIcon>
#include <QWebEngineView>

class CalendarView : public QWebEngineView
{
    Q_OBJECT

public:
    CalendarView();
    ~CalendarView();

private:
    QSettings* m_appSettings = nullptr;
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    bool m_showWhenStarted;
    void SetupTrayIcon();
    void InitializeSettings();
};
#endif // CALENDARVIEW_HPP