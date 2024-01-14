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
    std::cout<<str_response<<std::endl;

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
FrozenLake<side_size>::FrozenLake(const std::string& url)
:
 url_(url),
 is_slippery_(true),
 is_created_(false)
 {}

template<uint_t side_size>
FrozenLake<side_size>::FrozenLake(const std::string& url,
                                  const std::string& version,
                                  bool slippery)
    :
      url_(url),
      is_slippery_(slippery),
      is_created_(false)
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

    return FrozenLake<side_size>::create_time_step_from_response_(response);

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
//
//     if(this->current_state.last()){
//         return reset();
//     }
//

    const auto request_url = url_ + "/frozen-lake-env/step";
    http::Request request{request_url};

    auto body = std::to_string(action);

    const auto response = request.send("POST", body);
    return FrozenLake<side_size>::create_time_step_from_response_(response);

}

template<uint_t side_size>
typename FrozenLake<side_size>::dynamics_t
FrozenLake<side_size>::p(uint_t sidx, uint_t aidx)const{

#ifdef GYMFCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif

//     std::string s = "dynamics = " + FrozenLake::py_env_name + ".P["+std::to_string(sidx)+"]";
//     s += "["+std::to_string(aidx)+"]";
//     str_t exe_str = s.c_str();
//
//     // get the dynamics
//     boost::python::exec(exe_str, this->gym_namespace);
//     auto dynamics_list = boost::python::extract<boost::python::list>(this->gym_namespace["dynamics"]);
//
//     dynamics_t dyn;
//     dyn.reserve(boost::python::len(dynamics_list));
//
//     for(uint_t i=0; i < boost::python::len(dynamics_list); ++i){
//         auto dynamics_tuple = boost::python::extract<boost::python::tuple>(dynamics_list()[i]);
//         auto prob = boost::python::extract<real_t>(dynamics_tuple()[0]);
//         auto next_state = boost::python::extract<uint_t>(dynamics_tuple()[1]);
//         auto reward = boost::python::extract<real_t>(dynamics_tuple()[2]);
//         auto done = boost::python::extract<bool>(dynamics_tuple()[3]);
//         dyn.push_back(std::make_tuple(prob(), next_state(), reward(), done()));
//     }
//
//     return dyn;
    return dynamics_t();
}

template class FrozenLake<4>;
template class FrozenLake<8>;

}
}
}
