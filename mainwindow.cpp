#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include "optionwindow.h"
#include "startwindow.h"
#include "settingwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Récupération du QStackedWidget et de la page menu
    stackedWidget = ui->centralwidget->findChild<QStackedWidget*>("stackedWidget");
    pageMenu = stackedWidget->findChild<QWidget*>("pageMenu");

    // Création des pages
    startPage = new StartWindow(this);
    optionPage = new OptionWindow(this);
    settingPage = new SettingWindow(this);

    // Ajout des pages au QStackedWidget
    stackedWidget->addWidget(startPage);      // index 1
    stackedWidget->addWidget(optionPage);     // index 2
    stackedWidget->addWidget(settingPage);    // index 3

    // Connexions menu principal
    connect(ui->centralwidget->findChild<QPushButton*>("pushButtonQuitter"), &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->centralwidget->findChild<QPushButton*>("pushButtonStart"), &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(startPage);
    });
    connect(ui->centralwidget->findChild<QPushButton*>("pushButtonOption"), &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(optionPage);
    });
    connect(ui->centralwidget->findChild<QPushButton*>("pushButtonRules"), &QPushButton::clicked, this, [this]() {
        // À compléter si tu veux une page de règles
    });

    // Connexions depuis OptionWindow
    if (optionPage->findChild<QPushButton*>("pushButton")) {
        connect(optionPage->findChild<QPushButton*>("pushButton"), &QPushButton::clicked, this, [this]() {
            stackedWidget->setCurrentWidget(pageMenu);
        });
    }

    // Connexions depuis StartWindow
    connect(startPage->findChild<QPushButton*>("pushButtonBack"), &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(pageMenu);
    });
    connect(startPage->findChild<QPushButton*>("pushButtonSettings"), &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(settingPage);
    });

    // Connexions depuis SettingWindow
    connect(settingPage->findChild<QPushButton*>("pushButtonBack"), &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentWidget(startPage);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
