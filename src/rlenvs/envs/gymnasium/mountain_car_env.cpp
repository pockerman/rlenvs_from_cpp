#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/mountain_car_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/names_generator.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <iostream>

namespace rlenvs_cpp{
namespace gymfcpp{


const std::string MountainCarData::name = "MountainCar";

MountainCarData::state_type
MountainCarData::state_transform_from_boost(state_boost_python_type boost_type){

    MountainCarData::state_type obs;
    obs.reserve(boost::python::len(boost_type));

    for(auto i=0; i<boost::python::len(boost_type); ++i){
        obs.push_back(boost::python::extract<real_t>(boost_type[i]));
    }

    return obs;

}



MountainCarData::state_type
MountainCarData::extract_state_from_reset(obj_t gym_namespace, std::string py_state_name, std::string result_name){

    std::string s = py_state_name +   " = " +  result_name + ".tolist()\n";
    boost::python::exec(s.c_str(), gym_namespace);

    auto obs =  boost::python::extract<MountainCarData::state_boost_python_type>(gym_namespace[py_state_name]);
    return MountainCarData::state_transform_from_boost(obs);

}


MountainCar::MountainCar(std::string version, obj_t gym_namespace, bool do_create)
    :
      EnvMixin<MountainCarData>(version, gym_namespace)
{

    this->py_env_name = get_py_env_name(MountainCarData::name);
    this->py_reset_result_name = get_py_reset_rslt_name(MountainCarData::name);
    this->py_step_result_name = get_py_step_rslt_name(MountainCarData::name);
    this->py_state_name = get_py_state_name(MountainCarData::name);

    if(do_create){
        make();
    }
}

MountainCar::~MountainCar(){
    close();
}

void
MountainCar::make(){

    if(is_created){
        return;
    }

    std::string cpp_str = "import gym \n";
    cpp_str += "import numpy as np \n";
    cpp_str += this->py_env_name + " = gym.make('" + MountainCarData::name + "-" + version + "').unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    env = boost::python::extract<boost::python::api::object>(gym_namespace[this->py_env_name]);
    is_created = true;

}

MountainCar::time_step_type
MountainCar::step(action_type action){

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
    assert(action < n_actions() && "Invalid action index");
#endif

    if(current_state.last()){
        return reset();
    }

    std::string s = this->py_step_result_name + " = " + this->py_env_name +".step("+std::to_string(action)+")\n";
    s += this->py_step_result_name + " = (" + this->py_step_result_name + "[0].tolist(),";
    s += "float(" + this->py_step_result_name + "[1]),";
    s += this->py_step_result_name + "[2])\n";

    // execute the cmd on the python interpreter
    boost::python::exec(s.c_str(), gym_namespace);

    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace[this->py_step_result_name]);

    auto obs_boost =  boost::python::extract<boost::python::list>(result()[0]);
    auto obs = MountainCarData::state_transform_from_boost(obs_boost);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    this->current_state = time_step_type(done ? TimeStepTp::LAST : TimeStepTp::MID, reward, obs, std::move(extra));
    return current_state;

}

}
}
