#include "rlenvs/envs/gymnasium/classic_control/vector/acrobot_vec_env.h"
#include "rlenvs/envs/vector_time_step.h"
#include "rlenvs/envs/time_step_type.h"
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
GymnasiumVecEnvBase<VectorTimeStep<detail_::AcrobotVEnv::state_type>, detail_::AcrobotVEnv>(0, "Acrobot",
                                                                                   api_base_url,
                                                                                    "/gymnasium/acrobot-env/v")
{}

AcrobotV::AcrobotV(const std::string& api_base_url, const uint_t cidx)
:
GymnasiumVecEnvBase<VectorTimeStep<detail_::AcrobotVEnv::state_type>, 
                    detail_::AcrobotVEnv>(cidx, 
										 "Acrobot",
                                         api_base_url,
                                         "/gymnasium/acrobot-env/v")
{}

void
AcrobotV::make(const std::string& version,
              const std::unordered_map<std::string, std::any>& options){

    if(this->is_created()){
        return;
    }
	
	this->GymnasiumVecEnvBase<VectorTimeStep<detail_::AcrobotVEnv::state_type>,
                             detail_::AcrobotVEnv>::make(version, options);

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

    this->set_version_(version);
    this->make_created_();
}


AcrobotV::time_step_type
AcrobotV::step(const action_type& action){

	
#ifdef RLENVSCPP_DEBUG
     assert(this->is_created() && "Environment has not been created");
#endif

     if(this->get_reset_if_any_done() && this->get_current_time_step_().last()){
         return this->reset(42, std::unordered_map<std::string, std::any>());
     }

    const auto request_url = std::string(this->get_url()) + "/step";
    http::Request request{request_url};

	using json = nlohmann::json;
	json j;
    j["actions"] = action;
	j["cidx"] = this -> cidx();
	
    const auto response = request.send("POST", j.dump());

    if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();
}


std::unique_ptr<AcrobotV::base_type> 
AcrobotV::make_copy(uint_t cidx)const{
	auto api_base_url = this -> get_api_url();
	
	auto ptr = std::unique_ptr<AcrobotV::base_type>(new AcrobotV(api_base_url, cidx));
	
	std::unordered_map<std::string, std::any> ops;
	ops["num_envs"] = this -> get_n_envs();
	auto version = this -> version();
	ptr -> make(version, ops);
	return ptr;
}
	
}
}
}