#include "rlenvs/envs/gymnasium/toy_text/cliff_world_env.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>
#include <any>
#include <memory>


namespace rlenvscpp{
namespace envs {
namespace gymnasium{

const std::string CliffWorld::name = "CliffWalking";


CliffWorld::dynamics_t
CliffWorld::build_dynamics_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;
    json j = json::parse(str_response);

    auto dynamics = j["dynamics"];
    return dynamics;
}

CliffWorld::time_step_type
CliffWorld::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
	
	std::unordered_map<std::string, std::any> info_;
	info_["prob"] = std::any(static_cast<real_t>(info["prob"]));
	
	
    return CliffWorld::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
												 std::move(info_));
                                                
}


CliffWorld::CliffWorld(const std::string& api_base_url)
    :
ToyTextEnvBase<TimeStep<uint_t>, 
			   DiscreteEnv<37, 4>>(0, "CliffWalking", 
			                      api_base_url,
								  "/gymnasium/cliff-walking-env"),
max_episode_steps_(200)
{}

CliffWorld::CliffWorld(const std::string& api_base_url, 
	                   const uint_t cidx)
:
ToyTextEnvBase<TimeStep<uint_t>, 
			   DiscreteEnv<37, 4>>(cidx, "CliffWalking", 
			                      api_base_url,
								  "/gymnasium/cliff-walking-env"),
max_episode_steps_(200)
{}


void
CliffWorld::make(const std::string& version,
                const std::unordered_map<std::string, std::any>& options){

    if(this->is_created()){
        return;
    }
	
	auto max_episode_steps_itr = options.find("max_episode_steps");
    if( max_episode_steps_itr != options.end()){
        max_episode_steps_ = std::any_cast<uint_t>(max_episode_steps_itr->second);
    }

    const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["version"] = version;
	j["max_episode_steps"] = max_episode_steps_;
	j["cidx"] = this -> cidx();
    auto body = j.dump();

    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

    this->set_version_(version);
    this->make_created_();

}

CliffWorld::time_step_type
CliffWorld::step(const action_type& action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created() && "Environment has not been created");
#endif

     if(this->get_current_time_step_().last()){
         return this->reset(42, std::unordered_map<std::string, std::any>());
     }

    const auto request_url = std::string(this->get_url()) + "/step";
    http::Request request{request_url};

    auto copy_idx = this -> cidx();
   
	using json = nlohmann::json;
    json j;
	j["cidx"] = copy_idx;
	j["action"] = action;
    const auto response = request.send("POST", j.dump());

    if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();

}

std::unique_ptr<CliffWorld::base_type> 
CliffWorld::make_copy(uint_t cidx)const{
	
	auto api_base_url = this -> get_api_url();
	return std::make_unique<CliffWorld>(api_base_url,
								        cidx);
												   
}

}
}
}
