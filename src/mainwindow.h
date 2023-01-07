#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "protons.h"
#include "ge_proton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void add_text(const std::string& mytext);
    void remove_text();
    void updateProgressBar(int x);

private slots:
    void on_updateButton_clicked();

    void when_ge_proton_download_start();
    void when_ge_proton_download_perc_inc(int val);
    void when_ge_proton_download_finished(int success);

private:
    Ui::MainWindow *ui;
    protons p_;
    ge_proton ge_p_;

    void setupUiExtra();
    void disableButtons();
    void enableButtons();

    void check_installed_protons();
//    void update_protons();
};
#endif // MAINWINDOW_H
