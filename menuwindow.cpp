#include "menuwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include "mainwindow.h"
#include "settingswindow.h"
//#include "settingswindow.h" // à créer

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
{
    startButton = new QPushButton("Start", this);
    settingsButton = new QPushButton("Settings", this);
    quitButton = new QPushButton("Quit", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(startButton);
    layout->addWidget(settingsButton);
    layout->addWidget(quitButton);
    setLayout(layout);
    setWindowTitle("Menu Principal");

    connect(startButton, &QPushButton::clicked, this, &MenuWindow::onStartClicked);
    connect(settingsButton, &QPushButton::clicked, this, &MenuWindow::onSettingsClicked);
    connect(quitButton, &QPushButton::clicked, this, &MenuWindow::onQuitClicked);
}

void MenuWindow::onStartClicked()
{
    MainWindow *w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    this->hide();
    connect(w, &QWidget::destroyed, this, [this]() { this->show(); });
}

void MenuWindow::onSettingsClicked()
{
    SettingsWindow *settings = new SettingsWindow();
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->show();
    this->hide();
    connect(settings, &QWidget::destroyed, this, [this]() { this->show(); });
}

void MenuWindow::onQuitClicked()
{
    QApplication::quit();
} 