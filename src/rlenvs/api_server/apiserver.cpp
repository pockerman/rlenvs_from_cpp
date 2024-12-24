// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#include "rlenvs/api_server/apiserver.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#include <string>

namespace rlenvscpp{

ApiServerWrapper::ApiServerWrapper(const std::string& url)
:
url_(url)
{}

bool
ApiServerWrapper::has_gymnasium()const{

    const auto request_url = url_ + "/api-info/gymnasium";
    http::Request request{request_url};

    const auto response = request.send("GET");
    return response.status.code == 200;

}

std::vector<std::string>
ApiServerWrapper::gymnasium_envs()const{

    const auto request_url = url_ + "/api-info/gymnasium/envs";
    http::Request request{request_url};

    const auto response = request.send("GET");

    if(response.status.code != 200){
        throw std::runtime_error("Environment server responded with error");
    }

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;
    json j = json::parse(str_response);
    return j["envs"];
}
}
