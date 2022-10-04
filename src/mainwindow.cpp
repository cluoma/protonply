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
MainWindow::add_text(const std::string& mytext)
{
    ui->protonsList->addItem(QString(std::string(mytext).c_str()));
}

void
MainWindow::remove_text()
{
    ui->protonsList->removeItemWidget(ui->protonsList->item(0));
}
void
MainWindow::on_pushButton_clicked()
{
    auto row = ui->protonsList->takeItem(0);
    delete row;
}


void MainWindow::on_pushButton_pressed()
{
    std::cout << "aaa" << std::endl;
}


void MainWindow::on_pushButton_2_pressed()
{
    QApplication::quit();
}

