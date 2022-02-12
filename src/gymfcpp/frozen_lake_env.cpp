#include "gymfcpp/frozen_lake_env.h"
#include "gymfcpp/names_generator.h"
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


std::string FrozenLakeData::name = "FrozenLake";



FrozenLake::FrozenLake(const std::string& version, obj_t main_namespace,
                       bool do_create,  std::string map_type, bool is_slippery)
    :
      EnvMixin<FrozenLakeData>(version, main_namespace),
      map_type_(map_type),
      is_slippery_(is_slippery)
{
    this->py_env_name = get_py_env_name(FrozenLakeData::name);
    this->py_reset_result_name = get_py_reset_rslt_name(FrozenLakeData::name);
    this->py_step_result_name = get_py_step_rslt_name(FrozenLakeData::name);
    this->py_state_name = get_py_state_name(FrozenLakeData::name);

    if(do_create){
        make();
    }
}

FrozenLake::~FrozenLake(){
    close();
}



void
FrozenLake::make(){

    if(is_created){
        return;
    }

    /*std::string  python_str = "import gym \n";
    python_str +=  FrozenLake::py_env_name + " = gym.make('FrozenLake-v0', is_slippery=True).unwrapped \n";

    if(!is_slipery_){
        python_str = "import gym \n";
        python_str +=  FrozenLake::py_env_name + " = gym.make('FrozenLake-v0', is_slippery=False).unwrapped \n";
    }*/
    std::string cpp_str = construct_python_string_();

    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    env = boost::python::extract<boost::python::api::object>(gym_namespace[FrozenLake::py_env_name]);
    is_created = true;

    // create an environment
    /*auto ignored = boost::python::exec(python_str.c_str(), gym_namespace_);
    world_ = boost::python::extract<boost::python::api::object>(gym_namespace_["frozen_env"]);
    is_created_ = true;*/
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


/*uint_t
FrozenLake::n_actions()const{

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    auto world_dict = boost::python::extract<boost::python::dict>(world_.attr("__dict__"));
    auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);
    return boost::python::extract<uint_t>(action_space().attr("__dict__")["n"]);
}*/

FrozenLake::time_step_type
FrozenLake::reset(){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    // reset the python environment
    boost::python::exec("observation = frozen_env.reset()", gym_namespace_);

    // the observation
    auto observation =  boost::python::extract<uint_t>(gym_namespace_["observation"]);

    current_state_ = time_step_t(TimeStepTp::FIRST, 0.0, observation);
    return current_state_;

}


FrozenLake::time_step_type
FrozenLake::step(action_type action, bool query_extra){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    if(current_state_.last()){
        return reset();
    }

    std::string s = "result = " + FrozenLake::py_env_name +".step("+std::to_string(action)+")";

    // create an environment
    boost::python::exec(s.c_str(), gym_namespace_);

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

FrozenLake::dynamics_t
FrozenLake::p(uint_t sidx, uint_t aidx)const{

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    std::string s = "dynamics = " + FrozenLake::py_env_name + ".P["+std::to_string(sidx)+"]";
    s += "["+std::to_string(aidx)+"]";
    str_t exe_str = s.c_str();

    // get the dynamics
    boost::python::exec(exe_str, gym_namespace_);
    auto dynamics_list = boost::python::extract<boost::python::list>(gym_namespace_["dynamics"]);



    dynamics_t dyn;
    dyn.reserve(boost::python::len(dynamics_list));

    for(uint_t i=0; i < boost::python::len(dynamics_list); ++i){
        auto dynamics_tuple = boost::python::extract<boost::python::tuple>(dynamics_list()[i]);
        auto prob = boost::python::extract<real_t>(dynamics_tuple()[0]);
        auto next_state = boost::python::extract<uint_t>(dynamics_tuple()[1]);
        auto reward = boost::python::extract<real_t>(dynamics_tuple()[2]);
        auto done = boost::python::extract<bool>(dynamics_tuple()[3]);
        dyn.push_back(std::make_tuple(prob(), next_state(), reward(), done()));
    }


    return dyn;
}


void
FrozenLake::render(std::string mode){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    auto str = FrozenLake::py_env_name + ".render(mode=" + mode + ")\n";
    boost::python::exec(str.c_str(), gym_namespace_);

}

std::string
FrozenLake::construct_python_string_()const noexcept{

    std::string cpp_str = "import gym \n";

    if(is_slippery()){

        if(map_type() == "4x4"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData::name +"-" + version + "', map_name='4x4', is_slippery=True).unwrapped\n";
        }
        else if(map_type() == "8x8"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData::name +"-" + version + "', map_name='8x8', is_slippery=True).unwrapped\n";
        }
    }
    else{

        if(map_type() == "4x4"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData::name +"-" + version + "', map_name='4x4', is_slippery=False).unwrapped\n";
        }
        else if(map_type() == "8x8"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData::name +"-" + version + "', map_name='8x8', is_slippery=False).unwrapped\n";
        }
    }


    return cpp_str;

}

/*void
FrozenLake::close(){

    if(!is_created_){
        return;
    }

    auto str = FrozenLake::py_env_name + ".close()\n";
    boost::python::exec(str.c_str(), gym_namespace_);

}*/



}
