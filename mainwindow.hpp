#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QToolBar>

namespace Ui { class MainWindow; }

class GooglePageView;

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
    GooglePageView* m_googlePage = nullptr;
    void InitializeGooglePage();
    QMenuBar* m_menuBar = nullptr;
    void InitializeMenuBar();
    QToolBar* m_toolBar = nullptr;
    void InitializeToolBar();
    QSystemTrayIcon* m_trayIcon = nullptr;
    void SetupTrayIcon();
    void closeEvent(QCloseEvent* e) override;

signals:
    void SwitchShowHide();
};

#endif // MAINWINDOW_HPP
