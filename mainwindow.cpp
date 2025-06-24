#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include "optionwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , optionWindow(new OptionWindow(this)) // ✅ initialisation ici
{
    ui->setupUi(this);

    connect(ui->pushButtonQuitter, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(ui->pushButtonOption, &QPushButton::clicked, this, [this]() {
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
