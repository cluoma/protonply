#include "mainwindow.h"
#include "protons.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

int
check_steam_dirs(std::vector<std::string>& names)
{
    char * home_dir = getenv("HOME");
    if (home_dir == nullptr)
    {
        fprintf(stderr, "No home directory in environment\n");
        return 1;
    }

    auto home_dir_str = std::string(home_dir);
    std::filesystem::path steam_base_dir = home_dir_str;
    steam_base_dir /= ".steam/steam";

    // make compatibility directory if it doesn't exist
    if (!std::filesystem::exists(steam_base_dir / "compatibilitytools.d"))
    {
        fprintf(stderr, "No compatibilitytools.d dir found, creating it...\n");
        std::filesystem::create_directory(steam_base_dir / "compatibilitytools.d");
    }


    // look at all files in the compattools dir
    for (const auto & entry : std::filesystem::directory_iterator(steam_base_dir / "compatibilitytools.d"))
    {
        std::cout << entry.path() << std::endl;
        if (entry.is_directory())
        {
            std::cout << "directory" << std::endl;
            if (std::filesystem::exists(entry.path() / "version"))
            {
                std::cout << "found version file" << std::endl;
                std::ifstream version_file(entry.path() / "version");
                std::time_t timestamp;
                std::string name;
                version_file >> timestamp >> name;
                names.push_back(name);
                //std::cout << timestamp << " :: " << name << std::endl;

//                auto tp = std::chrono::system_clock::from_time_t(timestamp);
//                std::time_t tt = std::chrono::system_clock::to_time_t(tp);
//                std::cout << std::put_time(std::localtime(&tt), "%c %Z") << " :: " << name << std::endl;
            }
        }
    }

    return 0;
}

int
main(int argc, char *argv[])
{
//    std::vector<std::string> names;
//    check_steam_dirs(names);
//    std::cout << names.size() << "\n";
//    for (const auto& it : names)
//    {
//        std::cout << it << "\n";
//    }
    protons p;
    p.check_steam_dirs();
    p.print_protons();

    return 0;

    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/GloriousEggroll/proton-ge-custom/releases"});
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
    //std::cout << r.text << std::endl;

    json releases = json::parse(r.text);
//    releases["tag_name"].get<std::vector<std::string>>();
    std::cout << releases[0]["tag_name"] << std::endl;
    for (auto it : releases)
    {
        std::cout << "name: " << it["tag_name"] << '\n';
    }

    return 0;

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
    w.add_text("a");
    w.add_text("b");
    w.add_text("c");
    w.add_text("d");
    w.add_text("e");

    w.remove_text();
    w.show();
    return a.exec();
}
