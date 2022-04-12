#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QToolBar>

class GooglePageView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    QSettings* appSettings = nullptr;
    QSystemTrayIcon* GetTrayIcon() { return trayIcon; }

private:
    Ui::MainWindow* ui;
    void InitializeSettings();
    GooglePageView* googlePage = nullptr;
    void InitializeGooglePage();
    QMenuBar* menuBar = nullptr;
    void InitializeMenuBar();
    QToolBar* toolBar = nullptr;
    void InitializeToolBar();
    QSystemTrayIcon* trayIcon = nullptr;
    void SetupTrayIcon();
    void closeEvent(QCloseEvent* e) override;

signals:
    void SwitchShowHide();
};

#endif // MAINWINDOW_HPP