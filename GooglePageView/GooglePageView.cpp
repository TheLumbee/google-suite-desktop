#include "GooglePageView.hpp"

#include <QApplication>
#include <QFile>
#include <QMenu>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QVariant>
#include <QWebEngineCookieStore>
#include <QWebEngineNewWindowRequest>
#include <QWebEngineProfile>

GooglePageView::GooglePageView(QWidget* parent /*= nullptr*/) :
    m_mainWindow(qobject_cast<MainWindow*>(parent))
{
    setParent(parent);
    QFile* cookieFile = new QFile("cookieFile");
    cookieFile->open(QIODevice::ReadOnly);
    QWebEngineCookieStore* cookieStore = page()->profile()->cookieStore();
    connect(cookieStore, &QWebEngineCookieStore::cookieAdded, this, [=](const QNetworkCookie& cookie)
    {
        if (!cookie.isSessionCookie())
        {
            cookieFile->open(QIODevice::WriteOnly | QIODevice::Append);
            QString text = QString("%1\n").arg(cookie.toRawForm());
            cookieFile->write(text.toLatin1());
            cookieFile->close();
        }
    });

    if (cookieFile->exists() && cookieFile->size())
    {
        for (QNetworkCookie cookie : QNetworkCookie::parseCookies(cookieFile->readAll()))
        {
            cookieStore->setCookie(cookie, QUrl("https://accounts.google.com"));
        }
    }

    cookieFile->remove();
    setUrl(QUrl("https://mail.google.com"));
    InitializeSettings();

    connect(page(), &QWebEnginePage::newWindowRequested, this, [=](QWebEngineNewWindowRequest& request)
    {
        request.openIn(page());
    });
}

void GooglePageView::InitializeSettings()
{
    if (!m_mainWindow->m_appSettings->contains("ShowOnStartup"))
    {
        m_mainWindow->m_appSettings->setValue("ShowOnStartup", QVariant(true));
    }

    m_showOnStartup = m_mainWindow->m_appSettings->value("ShowOnStartup").toBool();
    m_mainWindow->m_appSettings->sync();
    setVisible(m_showOnStartup);
}
