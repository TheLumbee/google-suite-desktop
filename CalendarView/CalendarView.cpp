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
}

void CalendarView::InitializeSettings()
{
    if (!m_mainWindow->m_appSettings->contains("ShowOnStartup"))
    {
        m_mainWindow->m_appSettings->setValue("ShowOnStartup", QVariant(true));
    }

    m_showOnStartup = m_mainWindow->m_appSettings->value("ShowOnStartup").toBool();
    m_mainWindow->m_appSettings->sync();
    setVisible(m_showOnStartup);
}
