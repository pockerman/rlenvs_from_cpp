#include "gymfcpp/frozen_lake.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/config.h"

#include <boost/python.hpp>

#include <string>
#include <any>
#include <unordered_map>
#include <iostream>

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

namespace gymfcpp{


std::string FrozenLake::name = "FrozenLake";


FrozenLake::FrozenLake(std::string version, obj_t gym_namespace, bool do_create, bool is_slipery)
    :
      v_(version),
      is_created_(false),
      gym_namespace_(gym_namespace),
      world_(),
      current_state_()
{
    if(do_create){
        make(is_slipery);
    }
}



void
FrozenLake::make(bool is_slipery){

    //auto gym_module = boost::python::import("gym");
    //auto gym_namespace = gym_module.attr("__dict__");

    str_t python_str = "import gym \n"
                       "world = gym.make('FrozenLake-v0', is_slippery=True) \n"
                       "world = world.unwrapped";

    if(!is_slipery){
        python_str = "import gym \n"
                     "world = gym.make('FrozenLake-v0', is_slippery=False) \n"
                     "world = world.unwrapped";
    }

    // create an environment
    auto ignored = boost::python::exec(python_str, gym_namespace_);
    world_ = boost::python::extract<boost::python::api::object>(gym_namespace_["world"]);
    is_created_ = true;
}


uint_t
FrozenLake::n_states()const{
#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    auto world_dict = boost::python::extract<boost::python::dict>(world_.attr("__dict__"));
    auto observation_space = boost::python::extract<boost::python::api::object>(world_dict()["observation_space"]);
    return boost::python::extract<uint_t>(observation_space().attr("__dict__")["n"]);
}


uint_t
FrozenLake::n_actions()const{

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    auto world_dict = boost::python::extract<boost::python::dict>(world_.attr("__dict__"));
    auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);
    return boost::python::extract<uint_t>(action_space().attr("__dict__")["n"]);
}

TimeStep
FrozenLake::reset(){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    // create an environment
    boost::python::exec("observation = world.reset()", gym_namespace_);

    // the observation
    auto observation =  boost::python::extract<uint_t>(gym_namespace_["observation"]);

    current_state_ = TimeStep(TimeStepTp::FIRST, 0.0, observation);
    return current_state_;

}


TimeStep
FrozenLake::step(action_t action, bool query_extra){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    if(current_state_.last()){
        return reset();
    }

    std::string s = "result = world.step("+std::to_string(action)+")";
    str_t exe_str = s.c_str();

    // create an environment
    boost::python::exec(exe_str, gym_namespace_);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace_["result"]);

    auto observation = boost::python::extract<uint_t>(result()[0]);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    if(query_extra){

        auto prob_dict = boost::python::extract<boost::python::dict>(result()[3]);
        auto prob = boost::python::extract<real_t>(prob_dict()["prob"]);
        extra["prob"] = std::any(prob());
    }

    current_state_ = TimeStep(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), observation(), std::move(extra));
    return current_state_;

}





}
