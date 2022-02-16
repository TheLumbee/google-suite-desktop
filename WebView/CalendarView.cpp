#include "CalendarView.hpp"

#include <QApplication>
#include <QStandardPaths>
#include <QMenu>
#include <QVariant>

CalendarView::CalendarView()
{
    setUrl(QUrl("https://calendar.google.com"));
    InitializeSettings();
    SetupTrayIcon();
    setWindowTitle(tr("Google Calendar Linux"));
}

CalendarView::~CalendarView()
{
}

void CalendarView::SetupTrayIcon()
{
    trayIcon->setIcon(QIcon("Resources/google-calendar-icon.png"));
    QMenu* trayIconMenu = new QMenu(this);
    // Action to show/hide WebView
    QAction* action = trayIconMenu->addAction(m_showWhenStarted ?
        tr("Hide") : tr("Show"));

    // Alternate between show/hide text when triggered and show/hide WebView
    connect(action, &QAction::triggered, this, [=]
    {
        if (action->text() == tr("Show"))
        {
            setVisible(true);
            action->setText(tr("Hide"));
        }

        else
        {
            setVisible(false);
            action->setText(tr("Show"));
        }
    });
    
    // Checkable action to determine if main page should show when started
    action = trayIconMenu->addAction(tr("Minimize to tray on startup"));

    // Set correct setting when action is triggered
    connect(action, &QAction::triggered, this, [=]
    {
        m_showWhenStarted = !action->isChecked();
        m_appSettings->setValue("MinimizeToTray", QVariant::fromValue(m_showWhenStarted));
        m_appSettings->sync();
    });

    action->setCheckable(true);
    action->setChecked(!m_showWhenStarted);
    setVisible(!action->isChecked());

    // Action to quit application
    action = trayIconMenu->addAction(tr("Quit"));
    connect(action, &QAction::triggered, this, [=]
    {
        qApp->quit();
    });

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void CalendarView::InitializeSettings()
{
    QString iniPath;

#ifdef Q_OS_WIN
    iniPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "",
        QStandardPaths::LocateDirectory);
#else
    iniPath = QStandardPaths::locate(QStandardPaths::HomeLocation, "",
        QStandardPaths::LocateDirectory);
#endif //Q_OS_WIN

    m_appSettings = new QSettings(QString("%1/.google-calendar-desktop/AppSettings.ini")
        .arg(iniPath), QSettings::IniFormat);

    if (!m_appSettings->contains("MinimizeToTray"))
    {
        m_appSettings->setValue("MinimizeToTray", QVariant::fromValue(0));
    }

    m_showWhenStarted = m_appSettings->value("MinimizeToTray").toBool();
    m_appSettings->sync();
}
