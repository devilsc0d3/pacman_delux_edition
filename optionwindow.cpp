#include "optionwindow.h"
#include "ui_optionwindow.h"

OptionWindow::OptionWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OptionWindow)
{
    ui->setupUi(this);
}

OptionWindow::~OptionWindow()
{
    delete ui;
}
