//
// Created by colin on 29/09/22.
//

#include <fstream>
#include <utility>
#include <iostream>
#include "protons.h"

int protons::check_steam_dirs()
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

    // check there is a steam installation
    if (!std::filesystem::exists(steam_base_dir))
    {
        fprintf(stderr, "No Steam directory found");
        return 1;
    }

    // make compatibility directory if it doesn't exist
    if (!std::filesystem::exists(steam_base_dir / "compatibilitytools.d"))
    {
        fprintf(stderr, "No compatibilitytools.d dir found, creating it...\n");
        std::filesystem::create_directory(steam_base_dir / "compatibilitytools.d");
    }


    // look at all files in the compattools dir
    for (const auto & entry : std::filesystem::directory_iterator(steam_base_dir / "compatibilitytools.d"))
    {
        //std::cout << entry.path() << std::endl;
        if (entry.is_directory())
        {
            //std::cout << "directory" << std::endl;
            if (std::filesystem::exists(entry.path() / "version"))
            {
                //std::cout << "found version file" << std::endl;
                std::ifstream version_file(entry.path() / "version");
                std::time_t timestamp;
                std::string name;
                version_file >> timestamp >> name;

                if ( (version_file.rdstate() & std::ifstream::failbit ) != 0 )
                {
                    fprintf(stderr, "failed to parse version file\n");
                    continue;
                }

                add_install(entry.path(), name, timestamp);

//                auto tp = std::chrono::system_clock::from_time_t(timestamp);
//                std::time_t tt = std::chrono::system_clock::to_time_t(tp);
//                std::cout << std::put_time(std::localtime(&tt), "%c %Z") << " :: " << name << std::endl;
            }
        }
    }

    // sort so that the newest is first
    std::sort(protons_.begin(), protons_.end(),
              [](const struct install& a, const struct install& b) {return (a.time > b.time);}
    );
    std::cout << "Found " << protons_.size() << " proton installs\n";

    return 0;
}

void protons::add_install(std::filesystem::path dir, std::string name, std::time_t time)
{
    struct install tmp = {std::move(dir), std::move(name), time};
    protons_.push_back(tmp);
}

void protons::print_protons()
{
    for (const auto& it : protons_)
    {
        std::cout << it.dir.string() << " :: " << it.name << std::endl;
    }
}
