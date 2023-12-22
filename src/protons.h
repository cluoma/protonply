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
    int remove_install(std::string name);  // removes install from list and also deletes the folder

    std::vector<struct install>::iterator begin() { return protons_.begin(); }
    std::vector<struct install>::iterator end() { return protons_.end(); }
    [[nodiscard]] std::vector<struct install>::const_iterator begin() const { return protons_.begin(); }
    [[nodiscard]] std::vector<struct install>::const_iterator end() const { return protons_.end(); }
    [[nodiscard]] std::vector<struct install>::const_iterator cbegin() const { return protons_.cbegin(); }
    [[nodiscard]] std::vector<struct install>::const_iterator cend() const { return protons_.cend(); }

private:
    std::vector<struct install> protons_ = std::vector<struct install>();
    void add_install(std::filesystem::path dir, std::string name, std::time_t time);
};


#endif //PROTONPLY_PROTONS_H
