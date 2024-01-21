#include "rlenvs/envs/gymnasium/toy_text/taxi_env.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"


#ifdef RLENVSCPP_DEBUG
#include <cassert>
#include <iostream>
#endif

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium{

std::string TaxiData::name = "Taxi";


Taxi::Taxi(const std::string& api_base_url)
    :
     ToyTextEnvBase<TaxiData::time_step_type>(api_base_url + "/taxi-env")
{}

Taxi::dynamics_t
Taxi::build_dynamics_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;
    json j = json::parse(str_response);

    auto dynamics = j["dynamics"];
    return dynamics;
}

Taxi::time_step_type
Taxi::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return Taxi::time_step_type(time_step_type_from_int(step_type),
                                reward, observation, discount,
                                std::unordered_map<std::string, std::any>());
}

void
Taxi::make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& /*options*/){

     if(this->is_created()){
        return;
    }

    const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    auto body = "\""+version+"\"";
    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

    this->make_created();
}


Taxi::time_step_type
Taxi::step(TaxiActionsEnum action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

     if(this->get_current_time_step_().last()){
         return this->reset(42);
     }

    const auto request_url = std::string(this->get_url()) + "/step";
    http::Request request{request_url};

    auto body = std::to_string(action);

    const auto response = request.send("POST", body);

     if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();
}

}
}
}
