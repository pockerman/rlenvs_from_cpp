#include "rlenvs/envs/gymnasium/frozen_lake_env.h"
#include "rlenvs/names_generator.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/time_step_type.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#include <boost/tokenizer.hpp>
#include <string>
#include <any>
#include <unordered_map>
#include <iostream>
#include <random>
#include <unordered_map>

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium{

template<uint_t side_size>
const std::string FrozenLakeData<side_size>::name = "FrozenLake";


template<uint_t side_size>
typename FrozenLake<side_size>::time_step_type
FrozenLake<side_size>::create_time_step_from_response_(const http::Response& response){

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return FrozenLake<side_size>::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}


template<uint_t side_size>
typename FrozenLake<side_size>::dynamics_t
build_dynamics_from_response_(const http::Response& response){

    auto str_response = std::string(response.body.begin(), response.body.end());

    using json = nlohmann::json;
    json j = json::parse(str_response);

    auto dynamics = j["dynamics"];
    return dynamics;
}


template<uint_t side_size>
FrozenLake<side_size>::FrozenLake(const std::string& url)
:
 url_(url),
 is_slippery_(true),
 is_created_(false),
 current_state_()
 {}

template<uint_t side_size>
FrozenLake<side_size>::FrozenLake(const std::string& url,
                                  const std::string& version,
                                  bool slippery)
    :
      url_(url),
      is_slippery_(slippery),
      is_created_(false),
      current_state_()
{
    make(version, slippery);
}

template<uint_t side_size>
FrozenLake<side_size>::~FrozenLake(){
    close();
}

template<uint_t side_size>
void
FrozenLake<side_size>::close(){

     if(!is_created_){
        return;
    }

    http::Request request{url_ + "/frozen-lake-env/close"};
    const auto response = request.send("POST");
    is_created_ = false;

}

template<uint_t side_size>
bool
FrozenLake<side_size>::is_alive()const noexcept{

    http::Request request{url_ + "/frozen-lake-env/is-alive"};
    const auto response = request.send("GET");
    const auto str_response = std::string(response.body.begin(), response.body.end());
    auto pos = str_response.find("true");

    if (pos == std::string::npos){
        return false;
    }

    return true;
}

template<uint_t side_size>
typename FrozenLake<side_size>::time_step_type
FrozenLake<side_size>::reset(uint_t seed){

    if(!is_created_){
#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif
     return time_step_type();
    }

    const auto request_url = url_ + "/frozen-lake-env/reset";
    http::Request request{request_url};

    auto body = std::to_string(seed);
    const auto response = request.send("POST", body);

    current_state_ = FrozenLake<side_size>::create_time_step_from_response_(response);
    return current_state_;

}

template<uint_t side_size>
void
FrozenLake<side_size>::make(const std::string& version, bool slippery){

    if(is_created_){
        return;
    }

    is_slippery_ = slippery;
    const auto request_url = url_ + "/frozen-lake-env/make";
    http::Request request{request_url};

    //auto body = "'version':'v1','map_name':'4x4','is_slippery':true";
    auto body = "{\"version\": \"v1\", \"map_name\": \"4x4\", \"is_slippery\":true}";
    const auto response = request.send("POST", body);
    is_created_ = true;
}

template<uint_t side_size>
typename FrozenLake<side_size>::time_step_type
FrozenLake<side_size>::step(FrozenLakeActionsEnum action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

     if(current_state_.last()){
         return reset();
     }

    const auto request_url = url_ + "/frozen-lake-env/step";
    http::Request request{request_url};

    auto body = std::to_string(action);

    const auto response = request.send("POST", body);
    current_state_ = FrozenLake<side_size>::create_time_step_from_response_(response);
    return current_state_;

}

template<uint_t side_size>
typename FrozenLake<side_size>::dynamics_t
FrozenLake<side_size>::p(uint_t sidx, uint_t aidx)const{

#ifdef RLENVSCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif

    const auto request_url = url_ + "/frozen-lake-env/dynamics?stateId="+std::to_string(sidx)+"&actionId="+std::to_string(aidx);
    http::Request request{request_url};
    const auto response = request.send("GET");
    return build_dynamics_from_response_<side_size>(response);
}

template class FrozenLake<4>;
template class FrozenLake<8>;

}
}
}
