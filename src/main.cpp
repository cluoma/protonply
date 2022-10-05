#include "mainwindow.h"
#include "protons.h"
#include "ge_proton.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    protons p;
    if (p.check_steam_dirs())
    {
        return 1;
    }

    ge_proton ge_p;
    ge_p.check_for_releases();
    ge_p.print_releases();


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
    MainWindow w;
    // add all found installed proton versions
    for (const auto& it : p)
    {
        w.add_text(it.name);
    }

    // show window and start Qt
    w.show();
    return QApplication::exec();
}
