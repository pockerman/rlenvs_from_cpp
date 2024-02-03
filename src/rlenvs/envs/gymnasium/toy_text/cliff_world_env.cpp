#include "rlenvs/envs/gymnasium/toy_text/cliff_world_env.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif



namespace rlenvs_cpp{
namespace envs {
namespace gymnasium{

const std::string CliffWorldData::name = "CliffWalking";



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
    return CliffWorld::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}



CliffWorld::CliffWorld(const std::string& api_base_url)
    :
ToyTextEnvBase<CliffWorldData::time_step_type>(api_base_url + "/cliffwalking-env")
{}




void
CliffWorld::make(const std::string& version,
                const std::unordered_map<std::string, std::any>& options){

}

CliffWorld::time_step_type
CliffWorld::step(action_type action, bool query_extra){

#ifdef RLENVSCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif


}

CliffWorld::dynamics_type
CliffWorld::p(uint_t sidx, uint_t aidx)const{

#ifdef RLENVSCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif

}

}
}
}
