#include "CalendarView/CalendarView.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Google Calendar Desktop"));
    InitializeSettings();
    InitializeCalendar();
}

MainWindow::~MainWindow()
{
    delete ui;
    m_appSettings->setValue("WindowPosition", QVariant::fromValue(pos()));
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
    CalendarView* calendar = new CalendarView(this);
    connect(calendar, &CalendarView::CalendarOpened, this, &QMainWindow::show);
    connect(calendar, &CalendarView::CalendarClosed, this, &QMainWindow::hide);
    setCentralWidget(calendar);
    move(!m_appSettings->contains("WindowPosition") ?
        QApplication::primaryScreen()->geometry().center() - (rect().center() * 1.5) :
        m_appSettings->value("WindowPosition").toPoint());
    setVisible(calendar->IsShownOnStartup());
}
