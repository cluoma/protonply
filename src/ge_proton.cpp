//
// Created by colin on 04/10/22.
//

#include "ge_proton.h"
#include <iostream>

int ge_proton::check_for_releases() {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/GloriousEggroll/proton-ge-custom/releases?per_page=100"});
    // TODO: error check status code before continuing
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string

    json j = json::parse(r.text);
    for (auto it : j)
    {
        //std::cout << "name: " << it["tag_name"] << '\n';
        releases_.push_back(
            release {
                it["tag_name"].get<std::string>(),
                it["published_at"].get<std::string>()
            }
        );
        for (auto it2 : it["assets"])
        {
            if (it2["name"] == it["tag_name"].get<std::string>() + ".tar.gz")
            {
                releases_.back().asset_url = it2["browser_download_url"].get<std::string>();
            }
            else if (it2["name"] == it["name"].get<std::string>() + ".tar.gz")
            {
                releases_.back().asset_url = it2["browser_download_url"].get<std::string>();
            }
        }
    }

    // remove releases without a download url
    releases_.erase(
            std::remove_if(releases_.begin(), releases_.end(),
                           [](const release& x) { return x.asset_url.empty(); }),
                 releases_.end());

    return 0;
}

void ge_proton::print_releases() {
    for (const auto& it : releases_)
    {
        std::cout << it.tag_name << "    \n" << it.asset_url << std::endl;
    }
}
