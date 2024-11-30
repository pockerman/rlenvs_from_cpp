#include "rlens/envs/gymnasium/vector/acrobot_vec_env.h"


namespace rlenvs_cpp{
namespace envs{
namespace gymnasium{
	
const std::string AcrobotV::name = "Acrobot";
	
	
AcrobotV::AcrobotV(const std::string& api_base_url)
:
GymnasiumVecEnvBase<AcrobotV::time_step_type>(api_base_url + "/gymnasium/acrobot-env/v")
{}

void
AcrobotV::make(const std::string& version,
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


AcrobotV::time_step_type
AcrobotV::step(const AcrobotActionsEnum action){

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

AcrobotV::time_step_type
AcrobotV::step(uint_t action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

     auto action_enum = Acrobot::action_from_int(action);
     return step(action_enum);

}
	
}
}
}