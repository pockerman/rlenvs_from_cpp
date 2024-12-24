#include "rlenvs/envs/gymnasium/classic_control/pendulum_env.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/time_step.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"


namespace rlenvscpp{
namespace envs{
namespace gymnasium{
	
	// static data
const std::string Pendulum::name = "Pendulum";

Pendulum::time_step_type
Pendulum::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return Pendulum::time_step_type(time_step_type_from_int(step_type),
									reward, observation, discount,
									std::unordered_map<std::string, std::any>());
}


Pendulum::Pendulum(const std::string& api_base_url)
:
GymnasiumEnvBase<Pendulum::time_step_type>(api_base_url + "/gymnasium/pendulum-env")
{}

void
Pendulum::make(const std::string& version,
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


Pendulum::time_step_type
Pendulum::step(const Pendulum::action_type action){

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


}
}
}