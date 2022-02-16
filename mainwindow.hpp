#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

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
    void InitializeCalendar();
};

#endif // MAINWINDOW_HPP
