//
// Created by colin on 04/10/22.
//

#include "ge_proton.h"
#include "untar.h"

#include <iostream>

int ge_proton::check_for_releases(const protons& p) {
    emit check_for_releases_started();
    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/GloriousEggroll/proton-ge-custom/releases?per_page=100"});
    if (r.error.operator bool())
    {
        fprintf(stderr, "Error contacting github api\n");
        return 1;
    }

    if (r.status_code != 200)
    {
        fprintf(stderr, "Non-200 return status from github api\n");
        return 1;
    }

    json j = json::parse(r.text, nullptr, false, false);
    if (j.is_discarded())
    {
        fprintf(stderr, "Unable to parse github api response\n");
        return 1;
    }

    // populate vector of GE-Proton releases from JSON
    for (auto it : j)
    {
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
                releases_.back().asset_name = it2["name"].get<std::string>();
            }
            else if (it2["name"] == it["name"].get<std::string>() + ".tar.gz")
            {
                releases_.back().asset_url = it2["browser_download_url"].get<std::string>();
                releases_.back().asset_name = it2["name"].get<std::string>();
            }
        }
    }

    // remove releases without a download url
    releases_.erase(
            std::remove_if(releases_.begin(), releases_.end(),
                           [](const release& x) { return x.asset_url.empty(); }),
                 releases_.end());

    set_installed(p);

    emit check_for_releases_finished(has_update_available());

    return 0;
}

void ge_proton::print_releases() {
    for (const auto& it : releases_)
    {
        std::cout << it.tag_name << "    \n" << it.asset_url << std::endl;
    }
}

// search for and flag proton versions that are installed based on name
void ge_proton::set_installed(const protons& p) {
    for (const auto& it : p)
    {
        auto r = std::find_if(releases_.begin(), releases_.end(),
                  [&it](const release& x) { return x.tag_name == it.name; });
        if (r != releases_.end())
        {
            r->installed = true;
        }
    }
}

bool ge_proton::has_update_available() {
    if (!releases_[0].installed)
    {
        return true;
    }
    return false;
}

int ge_proton::download_latest() {
    if (releases_[0].installed)
    {
        return 1;
    }
    emit download_start();

    std::ofstream of(releases_[0].asset_name, std::ios::binary);
    cpr::Response r = cpr::Download(of, cpr::Url{releases_[0].asset_url},
                                    cpr::ProgressCallback([&](cpr::cpr_off_t downloadTotal, cpr::cpr_off_t downloadNow, cpr::cpr_off_t uploadTotal, cpr::cpr_off_t uploadNow, intptr_t userdata) -> bool
                                                          {
                                                              if (stop_download_)
                                                              {
                                                                  return false;
                                                              }
                                                              if (downloadTotal > 0)
                                                              {
                                                                  // send signal to main window
                                                                  emit download_inc_percent((int) (((float) downloadNow /
                                                                                               (float) downloadTotal) *
                                                                                              100));
                                                              }
                                                              return true;
                                                          }));
    std::cout << "http status code = " << r.status_code << std::endl << std::endl;

    if (!stop_download_ && r.status_code == 200) {
        emit download_finished(r.status_code == 200);
    } else {
        stop_download_ = false;
        emit download_finished(0);
    }

    return 0;
}

int ge_proton::install_latest() {
    if (releases_[0].installed)
    {
        return 1;
    }

    char * home_dir = getenv("HOME");
    std::filesystem::path steam_dir(home_dir);
    steam_dir = steam_dir / ".steam/steam/compatibilitytools.d";

    return untar(releases_[0].asset_name.c_str(), steam_dir.c_str());
}

void ge_proton::when_cancel_proton_update()
{
    stop_download_ = true;
}
