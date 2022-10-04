#include "mainwindow.h"
#include "protons.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <cpr/cpr.h>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

int
main(int argc, char *argv[])
{
    protons p;
    p.check_steam_dirs();

    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/GloriousEggroll/proton-ge-custom/releases"});
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
    std::cout << (int)r.error.code << std::endl;
    std::cout << r.text << std::endl;

    json releases = json::parse(r.text);
//    releases["tag_name"].get<std::vector<std::string>>();
    std::cout << releases[0]["tag_name"] << std::endl;
    for (auto it : releases)
    {
        std::cout << "name: " << it["tag_name"] << '\n';
    }

    //return 0;

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "protonply_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;

    for (const auto& it : p)
    {
        w.add_text(it.name);
    }

    w.show();
    return QApplication::exec();
}
