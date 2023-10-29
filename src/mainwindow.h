#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

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
    void on_protonsList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_removeProton_clicked();
    void on_quitButton_clicked();


    void when_ge_proton_check_for_releases_started();
    void when_ge_proton_check_for_releases_finished(int has_update);

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
    void remove_all_text();

};
#endif // MAINWINDOW_H
