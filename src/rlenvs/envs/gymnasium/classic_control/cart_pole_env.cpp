#include "rlenvs/envs/gymnasium/classic_control/cart_pole_env.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/time_step.h"
#include "rlenvs/envs/time_step_type.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>
#include <memory>

namespace rlenvscpp{
namespace envs {
namespace gymnasium{
	
const std::string CartPole::name = "CartPole";


CartPole::time_step_type
CartPole::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"].template get<uint_t>();
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return CartPole::time_step_type(TimeStepEnumUtils::time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}

CartPole::CartPole(const std::string& api_base_url)
:
GymnasiumEnvBase<TimeStep<std::vector<real_t> >,
				 ContinuousVectorStateDiscreteActionEnv<4, 2, 0, real_t >>(0, "CartPole",
                                                                             api_base_url,
																			 "/gymnasium/cart-pole-env")
{}

CartPole::CartPole(const std::string& api_base_url, 
				   const uint_t cidx)
:
GymnasiumEnvBase<TimeStep<std::vector<real_t> >,
				 ContinuousVectorStateDiscreteActionEnv<4, 2, 0, real_t >>(cidx, "CartPole",
                                                                                        api_base_url,
																			            "/gymnasium/cart-pole-env")
{}

void
CartPole::make(const std::string& version,
              const std::unordered_map<std::string, std::any>& /*options*/){

    if(this->is_created()){
        return;
    }

    const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["version"] = version;
	j["cidx"] = this -> cidx();

    auto body = j.dump();
    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

    this->set_version_(version);
    this->make_created_();
}


CartPole::time_step_type
CartPole::step(const action_type& action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created() && "Environment has not been created");
#endif

     if(this->get_current_time_step_().last()){
         return this->reset(42, std::unordered_map<std::string, std::any>());
     }

    const auto request_url = std::string(this->get_url()) + "/step";
    http::Request request{request_url};

	using json = nlohmann::json;
    json j;
	j["cidx"] = this -> cidx();;
	j["action"] = action;
	
    const auto response = request.send("POST", j.dump());

    if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();
}

std::unique_ptr<CartPole::base_type> 
CartPole::make_copy(uint_t cidx)const{
	auto api_base_url = this -> get_api_url();
	
	auto ptr = std::unique_ptr<CartPole::base_type>(new CartPole(api_base_url, cidx));
	std::unordered_map<std::string, std::any> ops;
	auto version = this -> version();
	ptr -> make(version, ops);
	return ptr;
}


}
}
}
