#include "rlenvs/envs/gymnasium/classic_control/vector/acrobot_vec_env.h"
#include "rlenvs/envs/gymnasium/classic_control/acrobot_env_actions_enum.h"
#include "rlenvs/vector_time_step.h"
#include "rlenvs/time_step_type.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#include <iostream>

namespace rlenvscpp{
namespace envs{
namespace gymnasium{
	

const std::string AcrobotV::name = "Acrobot";


AcrobotV::time_step_type
AcrobotV::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

	// parse the response
    json j = json::parse(str_response);

    auto step_types = j["time_step"]["step_types"].template get<std::vector<uint_t> >(); 
	auto time_step_types = TimeStepEnumUtils::time_step_type_from_int(step_types);
    auto reward_response = j["time_step"]["rewards"];
    auto discount_response = j["time_step"]["discounts"];
    auto observation = j["time_step"]["observations"];
    auto info = j["time_step"]["infos"];
    return AcrobotV::time_step_type(time_step_types,
									reward_response, 
									observation, 
									discount_response,
									std::unordered_map<std::string, std::any>());
}
	
	
AcrobotV::AcrobotV(const std::string& api_base_url)
:
GymnasiumVecEnvBase<AcrobotV::time_step_type>(api_base_url + "/gymnasium/acrobot-env/v")
{}

void
AcrobotV::make(const std::string& version,
              const std::unordered_map<std::string, std::any>& options){

    if(this->is_created()){
        return;
    }
	
	this->GymnasiumVecEnvBase<AcrobotV::time_step_type>::make(version, options);

    const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["version"] = version;
	j["num_envs"] = this->get_n_envs();

    auto body = j.dump();
    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

    this->set_version(version);
    this->make_created();
}


AcrobotV::time_step_type
AcrobotV::step(const std::vector<AcrobotActionsEnum>& actions){

	
#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

     if(this->get_reset_if_any_done() && this->get_current_time_step_().last()){
         return this->reset(42, std::unordered_map<std::string, std::any>());
     }

    const auto request_url = std::string(this->get_url()) + "/step";
    http::Request request{request_url};

	using json = nlohmann::json;
	json j;
    j["actions"] = AcrobotActionsEnumUtils::actions_to_ints(actions);
	auto body = j.dump();
	
    const auto response = request.send("POST", body);

    if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();
}

AcrobotV::time_step_type
AcrobotV::step(const std::vector<uint_t>& actions){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

     auto action_enum = AcrobotActionsEnumUtils::actions_from_ints(actions);
     return step(action_enum);

}

	
}
}
}