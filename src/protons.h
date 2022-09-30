//
// Created by colin on 29/09/22.
//

#ifndef PROTONPLY_PROTONS_H
#define PROTONPLY_PROTONS_H


#include <string>
#include <vector>
#include <ctime>
#include <filesystem>

class protons {
public:
    struct install {
        std::filesystem::path dir;
        std::string name;
        std::time_t time;
    };
    int check_steam_dirs();
    void print_protons();

private:
    std::vector<struct install> protons_ = std::vector<struct install>();
    void add_install(std::filesystem::path dir, std::string name, std::time_t time);
};


#endif //PROTONPLY_PROTONS_H
