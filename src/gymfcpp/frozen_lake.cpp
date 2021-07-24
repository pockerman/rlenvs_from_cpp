#include "gymfcpp/frozen_lake.h"
#include "config.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <boost/python.hpp>

namespace gymfcpp{


std::string FrozenLake::name = "FrozenLake";


FrozenLake::FrozenLake(std::string version, obj_t gym_namespace, bool do_create, bool is_slipery)
    :
      v_(version),
      is_created_(false),
      gym_namespace_(gym_namespace),
      world_()
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



}
