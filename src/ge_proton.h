//
// Created by colin on 04/10/22.
//

#ifndef PROTONPLY_GE_PROTON_H
#define PROTONPLY_GE_PROTON_H

#include <cpr/cpr.h>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class ge_proton {
public:
    struct release {
        std::string tag_name;
        std::string published_at;
        std::string asset_url;
        bool installed = false;
    };
    int check_for_releases();
    void print_releases();
private:
    std::vector<struct release> releases_;
};

#endif //PROTONPLY_GE_PROTON_H
