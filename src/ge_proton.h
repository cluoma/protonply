//
// Created by colin on 04/10/22.
//

#ifndef PROTONPLY_GE_PROTON_H
#define PROTONPLY_GE_PROTON_H

#include <QObject>

#include <cpr/cpr.h>
#include "nlohmann/json.hpp"
#include "protons.h"

using json = nlohmann::json;

class ge_proton  : public QObject {

Q_OBJECT

public:
    struct release {
        std::string tag_name;
        std::string published_at;
        std::string asset_url;
        std::string asset_name;
        bool installed = false;
    };
    int check_for_releases(const protons& p);
    void print_releases();
    void set_installed(const protons& p);
    bool has_update_available();
    int download_latest();
    int install_latest();

private:
    std::vector<struct release> releases_;

signals:
    void check_for_releases_started();
    void check_for_releases_finished(int has_update);


    void download_start();
    void download_inc_percent(int newValue);
    void download_finished(int success);
};

#endif //PROTONPLY_GE_PROTON_H
