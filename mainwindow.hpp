#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QSettings>
#include <QSystemTrayIcon>

namespace Ui { class MainWindow; }

class CalendarView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    QSettings* m_appSettings = nullptr;

private:
    Ui::MainWindow* ui;
    void InitializeSettings();
    CalendarView* m_calendar = nullptr;
    void InitializeCalendar();
    QMenuBar* m_menuBar = nullptr;
    void InitializeMenuBar();
    QSystemTrayIcon* m_trayIcon = nullptr;
    void SetupTrayIcon();
    void closeEvent(QCloseEvent* e) override;

signals:
    void SwitchShowHide();
};

#endif // MAINWINDOW_HPP
