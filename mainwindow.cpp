#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "rulesdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    RulesDialog rulesDialog(this);
    rulesDialog.exec();
}
