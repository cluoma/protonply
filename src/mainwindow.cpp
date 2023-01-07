#include <iostream>
#include <utility>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MainWindow::connect(&ge_p_, &ge_proton::download_inc_percent,
                     this, &MainWindow::when_ge_proton_download_perc_inc);
    MainWindow::connect(&ge_p_, &ge_proton::download_start,
                     this, &MainWindow::when_ge_proton_download_start);
    MainWindow::connect(&ge_p_, &ge_proton::download_finished,
                     this, &MainWindow::when_ge_proton_download_finished);

    ui->setupUi(this);
    setupUiExtra();

    QFuture<void> r = QtConcurrent::run( &MainWindow::check_installed_protons, this );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::check_installed_protons()
{
    p_ = protons();
    if (p_.check_steam_dirs() != 0)
    {
        //return 1;
    }
    // add all found installed proton versions
    for (const auto& it : p_)
    {
        add_text(it.name);
    }
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
//void
//MainWindow::on_pushButton_clicked()
//{
//    auto row = ui->protonsList->takeItem(0);
//    delete row;
//}

void MainWindow::setupUiExtra()
{
    // make progressBar invisible but keep its area reserved
    QSizePolicy sp_retain = ui->progressBar->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->progressBar->setSizePolicy(sp_retain);
    ui->progressBar->setVisible(false);

    // don't show the second box on the right of the window
    ui->userSettingsBox->setVisible(false);

    // setup progress bar
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
}

void MainWindow::updateProgressBar(int x)
{
    ui->progressBar->setValue(x);
}

void MainWindow::on_updateButton_clicked()
{
    disableButtons();

    ui->progressBar->setFormat("checking for new Protons...");
    ui->progressBar->setRange(0, 0);
    ui->progressBar->setVisible(true);

    if (ge_p_.check_for_releases() != 0)
    {
//        return 1;
    }
    //ge_p_.print_releases();
    ge_p_.set_installed(p_);
    if (ge_p_.has_update_available())
    {
        QFuture<int> r = QtConcurrent::run( &ge_proton::download_latest, &ge_p_);
        printf("downloaded");
    }
}

void MainWindow::disableButtons()
{
    ui->updateButton->setDisabled(true);
    QApplication::processEvents();
}

void MainWindow::enableButtons()
{
    ui->updateButton->setDisabled(false);
    QApplication::processEvents();
}

/* slots to update stuff while download a new Proton version */

void MainWindow::when_ge_proton_download_start()
{
    printf("GE-Proton update available\n");
    printf("downloading...");
    ui->progressBar->setRange(0, 100);
    ui->progressBar->reset();
    ui->progressBar->setFormat("downloading Proton - %p%");
}

void MainWindow::when_ge_proton_download_perc_inc(int val)
{
    if (ui->progressBar->value() != val)
    {
        printf("updated value\n");
        ui->progressBar->setValue(val);
    }
}

void MainWindow::when_ge_proton_download_finished(int success)
{
    ui->progressBar->setVisible(false);
    enableButtons();
}

