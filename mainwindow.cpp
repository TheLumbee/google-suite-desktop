#include "CalendarView/CalendarView.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopServices>
#include <QScreen>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_menuBar(new QMenuBar(this)),
    m_trayIcon(new QSystemTrayIcon(QIcon("Resources/google.svg")))
{
    ui->setupUi(this);
    setWindowTitle(tr("Google Suite Desktop"));
    InitializeSettings();
    m_calendar = new CalendarView(this);
    InitializeCalendar();
    SetupTrayIcon();
    InitializeMenuBar();
    qApp->setQuitOnLastWindowClosed(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    m_appSettings->setValue("WindowPosition", QVariant(pos()));
    m_appSettings->setValue("WindowSize", QVariant(size()));
    m_appSettings->sync();
}

void MainWindow::InitializeSettings()
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
}

void MainWindow::InitializeCalendar()
{
    connect(m_calendar, &CalendarView::CalendarOpened, this, &QMainWindow::show);
    connect(m_calendar, &CalendarView::CalendarClosed, this, &QMainWindow::hide);
    setCentralWidget(m_calendar);
    move(!m_appSettings->contains("WindowPosition") ?
        QApplication::primaryScreen()->geometry().center() - (rect().center() * 1.5) :
        m_appSettings->value("WindowPosition").toPoint());
    resize(!m_appSettings->contains("WindowSize") ?
        QSize(1200, 742) :
        m_appSettings->value("WindowSize").toSize());
    setVisible(m_calendar->m_showOnStartup);
}

void MainWindow::InitializeMenuBar()
{
    QMenu* menu = m_menuBar->addMenu(tr("File"));
    QAction* action = menu->addAction(tr("Quit Application"));
    connect(action, &QAction::triggered, this, [=]
    {
        qApp->quit();
    });

    menu = m_menuBar->addMenu(tr("Edit"));

    // Checkable action to determine if main page should show when started
    action = menu->addAction(tr("Minimize to tray on startup"));

    // Set correct setting when action is triggered
    connect(action, &QAction::triggered, this, [=]
    {
        m_calendar->m_showOnStartup = !action->isChecked();
        m_appSettings->setValue("ShowOnStartup", QVariant(m_calendar->m_showOnStartup));
        m_appSettings->sync();
    });

    action->setCheckable(true);
    action->setChecked(!m_calendar->m_showOnStartup);

    // Help menu
    menu = m_menuBar->addMenu(tr("Help"));
    action = menu->addAction(tr("Open Github page"));
    connect(action, &QAction::triggered, this, [=]
    {
        QDesktopServices::openUrl(QUrl("https://github.com/TheLumbee/google-suite-desktop"));
    });

    setMenuBar(m_menuBar);
}

void MainWindow::SetupTrayIcon()
{
    QMenu* trayIconMenu = new QMenu(this);

    // Action to show/hide WebView
    QAction* action = trayIconMenu->addAction(m_calendar->m_showOnStartup ?
        tr("Hide") : tr("Show"));

    // Alternate between show/hide text when triggered and show/hide WebView
    connect(action, &QAction::triggered, this, [=]
    {
        if (action->text() == tr("Show"))
        {
            m_calendar->show();
            emit m_calendar->CalendarOpened();
            action->setText(tr("Hide"));
        }

        else
        {
            m_calendar->hide();
            emit m_calendar->CalendarClosed();
            action->setText(tr("Show"));
        }
    });

    // Switch show/hide
    connect(this, &MainWindow::SwitchShowHide, this, [=]
    {
        action->setText(tr("Show"));
    });

    // Action to quit application
    action = trayIconMenu->addAction(tr("Quit"));
    connect(action, &QAction::triggered, this, [=]
    {
        qApp->quit();
    });

    m_trayIcon->setContextMenu(trayIconMenu);
    m_trayIcon->show();
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    Q_UNUSED(e)
    emit SwitchShowHide();
}
