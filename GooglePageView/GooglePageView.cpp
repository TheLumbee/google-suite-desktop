#include "GooglePageView.hpp"

#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QMenu>
#include <QStandardPaths>
#include <QtNetwork/qnetworkcookie.h>
#include <QtNetwork/qnetworkcookiejar.h>
#include <QVariant>
#include <QWebEngineCookieStore>
#include <QWebEngineNewWindowRequest>
#include <QWebEngineNotification>
#include <QWebEngineProfile>

GooglePageView::GooglePageView(QWidget* parent /*= nullptr*/) :
    mainWindow(qobject_cast<MainWindow*>(parent))
{
    setParent(parent);
    QString cookiePath;
#ifdef Q_OS_WIN
    cookiePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
    cookiePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#endif // Q_OS_WIN
    QFile* cookieFile = new QFile(QString("%1/.google-calendar-desktop/cookieFile.txt").arg(cookiePath));
    cookieFile->open(QIODevice::ReadOnly);
    QWebEngineCookieStore* cookieStore = page()->profile()->cookieStore();
    connect(cookieStore, &QWebEngineCookieStore::cookieAdded, this, [=](const QNetworkCookie& cookie)
    {
        if (!cookie.isSessionCookie())
        {
            cookieFile->open(QIODevice::WriteOnly | QIODevice::Append);
            cookieFile->write(QString("%1\n").arg(cookie.toRawForm()).toLatin1());
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
        QDesktopServices::openUrl(request.requestedUrl());
    });

    // Grant permission for notifications
    connect(page(), &QWebEnginePage::featurePermissionRequested, this, [=](const QUrl& securityOrigin, QWebEnginePage::Feature feature)
    {
        if (feature != QWebEnginePage::Notifications)
            return;
        page()->setFeaturePermission(securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
    });

    // How to present notifications
    page()->profile()->setNotificationPresenter([&](std::unique_ptr<QWebEngineNotification> notification)
    {
        mainWindow->GetTrayIcon()->showMessage(notification->title(), notification->message(), QSystemTrayIcon::NoIcon);
    });
}

void GooglePageView::InitializeSettings()
{
    if (!mainWindow->appSettings->contains("ShowOnStartup"))
    {
        mainWindow->appSettings->setValue("ShowOnStartup", QVariant(true));
    }

    showOnStartup = mainWindow->appSettings->value("ShowOnStartup").toBool();
    mainWindow->appSettings->sync();
    setVisible(showOnStartup);
}