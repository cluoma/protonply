#include <iostream>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

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

void
MainWindow::add_text(std::string mytext)
{
    ui->listWidget->addItem(QString(mytext.c_str()));
}

void
MainWindow::remove_text()
{
    ui->listWidget->removeItemWidget(ui->listWidget->item(0));
}
void
MainWindow::on_pushButton_clicked()
{
    ui->listWidget->takeItem(0);
}


void MainWindow::on_pushButton_pressed()
{
    std::cout << "aaa" << std::endl;
}

