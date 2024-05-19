#include "rlenvs/envs/gymnasium/classic_control/cart_pole_env.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/time_step.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>


namespace rlenvs_cpp{
namespace envs {
namespace gymnasium{

// static data
std::string CartPoleData::name = "CartPole";



CartPoleActionsEnum
CartPole::action_from_int(uint_t aidx){

    if(aidx==0)
        return CartPoleActionsEnum::LEFT;

    if(aidx==1)
        return CartPoleActionsEnum::RIGHT;

    return CartPoleActionsEnum::INVALID_ACTION;
}



CartPole::time_step_type
CartPole::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return CartPole::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}

CartPole::CartPole(const std::string& api_base_url)
:
GymnasiumEnvBase<CartPoleData::time_step_type>(api_base_url + "/cart-pole-env")
{}

void
CartPole::make(const std::string& version,
              const std::unordered_map<std::string, std::any>& options){

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


CartPole::time_step_type
CartPole::step(const CartPoleActionsEnum action){

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



CartPole::time_step_type
CartPole::step(uint_t action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

     auto action_enum = CartPole::action_from_int(action);
     return step(action_enum);

}

}
}
}
