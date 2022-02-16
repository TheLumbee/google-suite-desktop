#include "CalendarView.hpp"

#include <QApplication>
#include <QMenu>
#include <QVariant>

CalendarView::CalendarView(QWidget* parent /*= nullptr*/) :
    m_mainWindow(qobject_cast<MainWindow*>(parent))
{
    setParent(parent);
    setUrl(QUrl("https://calendar.google.com"));
    InitializeSettings();
    SetupTrayIcon();
}

CalendarView::~CalendarView()
{
}

void CalendarView::SetupTrayIcon()
{
    trayIcon->setIcon(QIcon("Resources/google-calendar-icon.png"));
    QMenu* trayIconMenu = new QMenu(this);

    // Action to show/hide WebView
    QAction* action = trayIconMenu->addAction(m_showOnStartup ?
        tr("Hide") : tr("Show"));

    // Alternate between show/hide text when triggered and show/hide WebView
    connect(action, &QAction::triggered, this, [=]
    {
        if (action->text() == tr("Show"))
        {
            setVisible(true);
            emit CalendarOpened();
            action->setText(tr("Hide"));
        }

        else
        {
            setVisible(false);
            emit CalendarClosed();
            action->setText(tr("Show"));
        }
    });
    
    // Checkable action to determine if main page should show when started
    action = trayIconMenu->addAction(tr("Minimize to tray on startup"));

    // Set correct setting when action is triggered
    connect(action, &QAction::triggered, this, [=]
    {
        m_showOnStartup = !action->isChecked();
        m_mainWindow->m_appSettings->setValue("MinimizeToTrayOnStartup", QVariant::fromValue(m_showOnStartup));
        m_mainWindow->m_appSettings->sync();
    });

    action->setCheckable(true);
    action->setChecked(!m_showOnStartup);
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
    if (!m_mainWindow->m_appSettings->contains("MinimizeToTrayOnStartup"))
    {
        m_mainWindow->m_appSettings->setValue("MinimizeToTrayOnStartup", QVariant::fromValue(0));
    }

    m_showOnStartup = m_mainWindow->m_appSettings->value("MinimizeToTrayOnStartup").toBool();
    m_mainWindow->m_appSettings->sync();
}
