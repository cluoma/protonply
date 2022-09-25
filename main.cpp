#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <filesystem>
#include <fstream>

int main(int argc, char *argv[])
{
    std::string path = "/home/colin/.steam/steam/compatibilitytools.d/";
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        std::cout << entry.path() << std::endl;
        if (entry.is_directory())
        {
            std::cout << "directory" << std::endl;
            if (std::filesystem::exists(entry.path() / "version"))
            {
                std::cout << "found version file" << std::endl;
                std::ifstream version_file(entry.path() / "version");
                int timestamp;
                std::string name;
                version_file >> timestamp >> name;
                std::cout << timestamp << " :: " << name << std::endl;
            }
        }
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
