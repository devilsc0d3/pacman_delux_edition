#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>

class QPushButton;

class MenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = nullptr);

private slots:
    void onStartClicked();
    void onSettingsClicked();
    void onQuitClicked();

private:
    QPushButton *startButton;
    QPushButton *settingsButton;
    QPushButton *quitButton;
};

#endif // MENUWINDOW_H 