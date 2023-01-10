#include "mainwindow.h"
#include "untar.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
//    untar("GE-Proton7-43.tar.gz", "/home/colin/Documents/protonply/test/");
//    return 0;


    // start application
    QApplication a(argc, argv);

    // load translations
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "protonply_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // start main window
    MainWindow w(nullptr);

    // show window and start Qt
    w.show();
    return QApplication::exec();
}
