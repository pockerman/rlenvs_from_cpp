#include "rlenvs/envs/gymnasium/toy_text/black_jack_env.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"
#include "rlenvs/rlenvscpp_config.h"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>
#include <typeinfo>

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium
{

const std::string BlackJackData::name = "BlackJack";


BlackJackActionsEnum
BlackJack::action_from_int(uint_t aidx){

    switch(aidx){

       case 0:
           return BlackJackActionsEnum::STICK;
       case 1:
           return BlackJackActionsEnum::HIT;
       default:
           return BlackJackActionsEnum::INVALID_ACTION;
   }


}


BlackJack::dynamics_t
BlackJack::build_dynamics_from_response_(const http::Response& response)const{
   throw std::logic_error("Environment BlackJack does not have dynamics");
}

BlackJack::time_step_type
BlackJack::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = time_step_type_from_int(j["time_step"]["step_type"]);
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];

    std::vector<std::tuple<uint_t, uint_t, uint_t> > state(1, observation);
    return BlackJack::time_step_type(step_type, reward, state, discount,
                                     std::unordered_map<std::string, std::any>());
}

BlackJack::BlackJack(const std::string& api_base_url)
    :
   ToyTextEnvBase<BlackJackData::time_step_type>(api_base_url + "/gymnasium/black-jack-env")
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

     /*if(this->get_current_time_step_().last()){
         return this->reset(42, std::unordered_map<std::string, std::any>());
     }*/

    //std::cout<<"Request URL: "<<this->get_url()<<std::endl;
    const std::string request_url = "http://0.0.0.0:8001/api//gymnasium/black-jack-env/step"; //+ //std::string(this->get_url()) + "/step";

    std::cout<<"Request URL: "<<request_url<<std::endl;

    /*http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["action"] = 0; //static_cast<int>(action); //._to_integral();

    auto body = j.dump();
    const auto response = request.send("POST", body);

    if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();*/



}


BlackJack::time_step_type
BlackJack::step(uint_t action){

   auto blackjack_action = BlackJack::action_from_int(action);
   return step(blackjack_action);
}
}
}
}
