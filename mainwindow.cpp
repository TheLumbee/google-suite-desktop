#include "GooglePageView/GooglePageView.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QScreen>
#include <QStandardPaths>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_menuBar(new QMenuBar(this)),
    m_trayIcon(new QSystemTrayIcon(QIcon("Resources/google.svg")))
{
    ui->setupUi(this);
    setWindowTitle(tr("Google Suite Desktop"));
    setWindowIcon(QIcon("Resources/google.svg"));
    InitializeSettings();
    m_googlePage = new GooglePageView(this);
    InitializeGooglePage();
    SetupTrayIcon();
    InitializeMenuBar();
    InitializeToolBar();
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
    iniPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
    iniPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#endif //Q_OS_WIN
    m_appSettings = new QSettings(QString("%1/.google-calendar-desktop/AppSettings.ini")
        .arg(iniPath), QSettings::IniFormat);
}

void MainWindow::InitializeGooglePage()
{
    connect(m_googlePage, &GooglePageView::PageOpened, this, &QMainWindow::show);
    connect(m_googlePage, &GooglePageView::PageClosed, this, &QMainWindow::hide);
    setCentralWidget(m_googlePage);
    move(m_appSettings->contains("WindowPosition") ? 
        m_appSettings->value("WindowPosition").toPoint() :
        QApplication::primaryScreen()->geometry().center() - (rect().center() * 1.5));
    resize(m_appSettings->contains("WindowSize") ?
        m_appSettings->value("WindowSize").toSize() :
        QSize(1200, 742));
    setVisible(m_googlePage->m_showOnStartup);
}

void MainWindow::InitializeMenuBar()
{
    QMenu* menu = m_menuBar->addMenu(tr("File"));
    QAction* action = menu->addAction(tr("Open Calendar"));
    connect(action, &QAction::triggered, this, [=]
    {
        if (action->text().contains(tr("Calendar")))
        {
            m_googlePage->setUrl(QUrl("https://calendar.google.com"));
            action->setText(tr("Open Gmail"));
        }
        
        else if (action->text().contains(tr("Gmail")))
        {
            m_googlePage->setUrl(QUrl("https://mail.google.com"));
            action->setText(tr("Open Calendar"));
        }
    });

    action = menu->addAction(tr("Quit Application"));
    connect(action, &QAction::triggered, this, [=]
    {
        qApp->quit();
    });

    menu = m_menuBar->addMenu(tr("Edit"));
    action = menu->addAction(tr("Refresh"));
    connect(action, &QAction::triggered, m_googlePage, &GooglePageView::reload);

    // Checkable action to determine if main page should show when started
    action = menu->addAction(tr("Minimize to tray on startup"));

    // Set correct setting when action is triggered
    connect(action, &QAction::triggered, this, [=]
    {
        m_googlePage->m_showOnStartup = !action->isChecked();
        m_appSettings->setValue("ShowOnStartup", QVariant(m_googlePage->m_showOnStartup));
        m_appSettings->sync();
    });

    action->setCheckable(true);
    action->setChecked(!m_googlePage->m_showOnStartup);

    // Help menu
    menu = m_menuBar->addMenu(tr("Help"));
    action = menu->addAction(tr("Open Github page"));
    connect(action, &QAction::triggered, this, [=]
    {
        QDesktopServices::openUrl(QUrl("https://github.com/TheLumbee/google-suite-desktop"));
    });

    setMenuBar(m_menuBar);
}

void MainWindow::InitializeToolBar()
{
    m_toolBar = new QToolBar();
    QPushButton* button = new QPushButton();
    button->setIcon(QIcon("Resources/back.png"));
    button->setToolTip(tr("Go Back"));
    connect(button, &QPushButton::clicked, m_googlePage, &GooglePageView::back);
    m_toolBar->addWidget(button);
    button = new QPushButton();
    button->setIcon(QIcon("Resources/refresh.png"));
    button->setToolTip(tr("Refresh Page"));
    connect(button, &QPushButton::clicked, m_googlePage, &GooglePageView::reload);
    m_toolBar->addWidget(button);
    addToolBar(m_toolBar);
}

void MainWindow::SetupTrayIcon()
{
    QMenu* trayIconMenu = new QMenu(this);

    // Action to show/hide WebView
    QAction* action = trayIconMenu->addAction(m_googlePage->m_showOnStartup ?
        tr("Hide") : tr("Show"));

    // Alternate between show/hide text when triggered and show/hide WebView
    connect(action, &QAction::triggered, this, [=]
    {
        if (action->text() == tr("Show"))
        {
            m_googlePage->show();
            emit m_googlePage->PageOpened();
            action->setText(tr("Hide"));
        }

        else
        {
            m_googlePage->hide();
            emit m_googlePage->PageClosed();
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
