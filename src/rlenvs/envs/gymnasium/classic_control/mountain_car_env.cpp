#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/envs/gymnasium/classic_control/mountain_car_env.h"
#include "rlenvs/time_step.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"
#include "rlenvs/extern/HTTPRequest.hpp"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <string>
#include <vector>
#include <tuple>
#include <any>

namespace rlenvscpp{
namespace envs{
namespace gymnasium{


const std::string MountainCar::name = "MountainCar";


MountainCar::time_step_type
MountainCar::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return MountainCar::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}


MountainCar::MountainCar(const std::string& api_base_url)
    :
      GymnasiumEnvBase<MountainCarData::time_step_type>(api_base_url + "/gymnasium/mountain-car-env")
{}


void
MountainCar::make(const std::string& version,
              const std::unordered_map<std::string, std::any>& /*options*/){

    if(this->is_created()){
        return;
    }

    const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["version"] = version;

    auto body = j.dump();
    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

    this->set_version(version);
    this->make_created();

}

MountainCar::time_step_type
MountainCar::step(MountainCarActionsEnum action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

     if(this->get_current_time_step_().last()){
         return this->reset(42, std::unordered_map<std::string, std::any>());
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

MountainCarActionsEnum
MountainCar::sample_action()const{

    auto action = sample_action_id();

    if (action == 0){
        return MountainCarActionsEnum::ACCELERATE_LEFT;
    }
    else if(action == 1){
        return MountainCarActionsEnum::DO_NOT_ACCELERATE;
    }
    else if(action == 2){
        return MountainCarActionsEnum::ACCELERATE_RIGHT;
    }

   throw std::runtime_error("Invalid action");
}

}
}
}
