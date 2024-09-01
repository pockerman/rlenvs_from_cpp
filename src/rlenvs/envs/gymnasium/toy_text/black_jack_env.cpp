#include "rlenvs/envs/gymnasium/toy_text/black_jack_env.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"
#include "rlenvs/rlenvscpp_config.h"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif


namespace rlenvs_cpp{
namespace envs{
namespace gymnasium
{

const std::string BlackJackData::name = "BlackJack";


BlackJack::dynamics_t
BlackJack::build_dynamics_from_response_(const http::Response& response)const{
   throw std::logic_error("Environment BlackJack does not have dynamics");
}

BlackJack::time_step_type
BlackJack::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return BlackJack::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}

BlackJack::BlackJack(const std::string& api_base_url)
    :
   ToyTextEnvBase<BlackJackData::time_step_type>(api_base_url + "/black-jack-env")
{}



void
BlackJack::make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options){

    if(this->is_created()){
        return;
    }

    auto natural_itr = options.find("natural");
    if( natural_itr != options.end()){
        is_natural_ = std::any_cast<bool>(natural_itr->second);
    }

    auto sab_itr = options.find("sab");
    if( sab_itr != options.end()){
        is_sab_ = std::any_cast<bool>(sab_itr->second);
    }

    const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["version"] = version;
    j["natural"]  = is_natural_;
    j["sab"] = is_sab_;

    auto body = j.dump();
    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

    this->set_version(version);
    this->make_created();

}

BlackJack::time_step_type
BlackJack::step(BlackJackActionsEnum action){

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


BlackJack::time_step_type
BlackJack::step(uint_t action){

   switch(action){

       case 0:
           return step(BlackJackActionsEnum::STICK);
       case 1:
           return step(BlackJackActionsEnum::HIT);
       default:
           throw std::logic_error("Invalid action selected");

   }
}
}
}
}
