#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "optionwindow.h"
#include <QMainWindow>
#include "startwindow.h"
#include "settingwindow.h"
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    QWidget *pageMenu;
    StartWindow *startPage;
    OptionWindow *optionPage;
    SettingWindow *settingPage;
};
#endif // MAINWINDOW_H

