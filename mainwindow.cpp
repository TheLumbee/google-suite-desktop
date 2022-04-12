#include "GooglePageView/GooglePageView.hpp"
#include "mainwindow.hpp"

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QScreen>
#include <QStandardPaths>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), menuBar(new QMenuBar(this)),
    trayIcon(new QSystemTrayIcon(QIcon("Resources/google.svg")))
{
    ui->setupUi(this);
    setWindowTitle(tr("Google Suite Desktop"));
    setWindowIcon(QIcon("Resources/google.svg"));
    InitializeSettings();
    googlePage = new GooglePageView(this);
    InitializeGooglePage();
    SetupTrayIcon();
    InitializeMenuBar();
    InitializeToolBar();
    qApp->setQuitOnLastWindowClosed(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    appSettings->setValue("WindowPosition", QVariant(pos()));
    appSettings->setValue("WindowSize", QVariant(size()));
    appSettings->sync();
}

void MainWindow::InitializeSettings()
{
    QString iniPath;
#ifdef Q_OS_WIN
    iniPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
    iniPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
#endif // Q_OS_WIN
    appSettings = new QSettings(QString("%1/.google-calendar-desktop/AppSettings.ini")
        .arg(iniPath), QSettings::IniFormat);
}

void MainWindow::InitializeGooglePage()
{
    connect(googlePage, &GooglePageView::PageOpened, this, &QMainWindow::show);
    connect(googlePage, &GooglePageView::PageClosed, this, &QMainWindow::hide);
    setCentralWidget(googlePage);
    move(appSettings->contains("WindowPosition") ?
        appSettings->value("WindowPosition").toPoint() :
        QApplication::primaryScreen()->geometry().center() - (rect().center() * 1.5));
    resize(appSettings->contains("WindowSize") ?
        appSettings->value("WindowSize").toSize() :
        QSize(1200, 742));
    setVisible(googlePage->showOnStartup);
}

void MainWindow::InitializeMenuBar()
{
    QMenu* menu = menuBar->addMenu(tr("File"));
    QAction* action = menu->addAction(tr("Open Calendar"));
    connect(action, &QAction::triggered, this, [=]
    {
        if (action->text().contains(tr("Calendar")))
        {
            googlePage->setUrl(QUrl("https://calendar.google.com"));
            action->setText(tr("Open Gmail"));
        }

        else if (action->text().contains(tr("Gmail")))
        {
            googlePage->setUrl(QUrl("https://mail.google.com"));
            action->setText(tr("Open Calendar"));
        }
    });

    action = menu->addAction(tr("Quit Application"));
    connect(action, &QAction::triggered, this, [=]
    {
        qApp->quit();
    });

    menu = menuBar->addMenu(tr("Edit"));
    action = menu->addAction(tr("Refresh"));
    connect(action, &QAction::triggered, googlePage, &GooglePageView::reload);

    // Checkable action to determine if main page should show when started
    action = menu->addAction(tr("Minimize to tray on startup"));

    // Set correct setting when action is triggered
    connect(action, &QAction::triggered, this, [=]
    {
        googlePage->showOnStartup = action->isChecked();
        appSettings->setValue("ShowOnStartup", QVariant(googlePage->showOnStartup));
        appSettings->sync();
    });

    action->setCheckable(true);
    action->setChecked(!googlePage->showOnStartup);

    // Help menu
    menu = menuBar->addMenu(tr("Help"));
    action = menu->addAction(tr("Open Github page"));
    connect(action, &QAction::triggered, this, [=]
    {
        QDesktopServices::openUrl(QUrl("https://github.com/TheLumbee/google-suite-desktop"));
    });

    setMenuBar(menuBar);
}

void MainWindow::InitializeToolBar()
{
    toolBar = new QToolBar();
    QPushButton* button = new QPushButton();
    button->setIcon(QIcon("Resources/back.png"));
    button->setToolTip(tr("Go Back"));
    connect(button, &QPushButton::clicked, googlePage, &GooglePageView::back);
    toolBar->addWidget(button);
    button = new QPushButton();
    button->setIcon(QIcon("Resources/refresh.png"));
    button->setToolTip(tr("Refresh Page"));
    connect(button, &QPushButton::clicked, googlePage, &GooglePageView::reload);
    toolBar->addWidget(button);
    addToolBar(toolBar);
}

void MainWindow::SetupTrayIcon()
{
    QMenu* trayIconMenu = new QMenu(this);

    // Action to show/hide WebView
    QAction* action = trayIconMenu->addAction(googlePage->showOnStartup ?
        tr("Hide") : tr("Show"));

    // Alternate between show/hide text when triggered and show/hide WebView
    connect(action, &QAction::triggered, this, [=]
    {
        if (action->text() == tr("Show"))
        {
            googlePage->show();
            emit googlePage->PageOpened();
            action->setText(tr("Hide"));
        }

        else
        {
            googlePage->hide();
            emit googlePage->PageClosed();
            action->setText(tr("Show"));
        }
    });

    connect(trayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason)
    {
        switch (reason)
        {
            case (QSystemTrayIcon::Trigger):
            {
                googlePage->show();
                emit googlePage->PageOpened();
                action->setText(tr("Hide"));
                break;
            }

            default:
                break;
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

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    Q_UNUSED(e)
    emit SwitchShowHide();
}