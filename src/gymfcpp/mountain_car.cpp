#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/mountain_car.h"
#include "gymfcpp/time_step.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <iostream>

namespace gymfcpp{


std::string MountainCarData::name = "MountainCar";
std::string MountainCarData::py_env_name = "py_mountain_car";
std::string MountainCarData::py_step_result_name = "py_mountain_car_step_rslt";
std::string MountainCarData::py_reset_result_name = "py_mountain_car_reset_rslt";
std::string MountainCarData::py_state_name = "py_mountain_car_state";

MountainCarData::state_t
MountainCarData::state_transform_from_boost(state_boost_python_t boost_type){

    MountainCarData::state_t obs;
    obs.reserve(boost::python::len(boost_type));

    for(auto i=0; i<boost::python::len(boost_type); ++i){
        obs.push_back(boost::python::extract<real_t>(boost_type[i]));
    }

    return obs;

}

MountainCarData::state_t
MountainCarData::extract_state(obj_t gym_namespace, std::string result_name){

    std::string s;
    if(result_name == MountainCarData::py_reset_result_name){
        s = MountainCarData::py_state_name +   " = " +  result_name + ".tolist()\n";
    }
    else if(result_name == MountainCarData::py_step_result_name){

        s = MountainCarData::py_state_name + " = " + result_name + "[0]\n";
    }
#ifdef GYMFCPP_DEBUG
    else{
        assert(false && result_name + "not " + MountainCarData::py_reset_result_name + " or " + MountainCarData::py_step_result_name);
    }
#endif

    boost::python::exec(s.c_str(), gym_namespace);

    auto obs =  boost::python::extract<MountainCarData::state_boost_python_t>(gym_namespace[MountainCarData::py_state_name]);
    return MountainCarData::state_transform_from_boost(obs);

}

MountainCar::MountainCar(std::string version, obj_t gym_namespace, bool do_create)
    :
      EnvMixin<MountainCarData>(version, gym_namespace)
{

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
    cpp_str += MountainCarData::py_env_name + " = gym.make('" + MountainCarData::name + "-" + version + "').unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    env = boost::python::extract<boost::python::api::object>(gym_namespace[MountainCarData::py_env_name]);
    is_created = true;

}

MountainCar::time_step_t
MountainCar::step(action_t action){

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
#endif

    if(current_state.last()){
        return reset();
    }

    std::string s = MountainCarData::py_step_result_name + " = " + MountainCarData::py_env_name +".step("+std::to_string(action)+")\n";
    s += MountainCarData::py_step_result_name + " = (" + MountainCarData::py_step_result_name + "[0].tolist(),";
    s += "float(" + MountainCarData::py_step_result_name + "[1]),";
    s += MountainCarData::py_step_result_name + "[2])\n";

    boost::python::exec(s.c_str(), gym_namespace);

    auto obs = MountainCarData::extract_state(gym_namespace, MountainCarData::py_step_result_name);

    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace[MountainCarData::py_step_result_name]);

    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    current_state = time_step_t(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), obs, std::move(extra));
    return current_state;

}

}
