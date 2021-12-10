#include "gymfcpp/mountain_car.h"
#include "gymfcpp/time_step.h"

namespace gymfcpp{


std::string MountainCarData::name = "MountainCar";
std::string MountainCarData::py_env_name = "py_mountain_car";
std::string MountainCarData::py_step_result_name = "py_mountain_car_step_rslt";
std::string MountainCarData::py_reset_result_name = "py_mountain_car_reset_rslt";

MountainCarData::state_t
MountainCarData::state_transform_from_boost(state_boost_python_t boost_type){

    MountainCarData::state_t obs;
    obs.reserve(boost::python::len(boost_type));

    for(auto i=0; i<boost::python::len(boost_type); ++i){
        obs.push_back(boost::python::extract<real_t>(boost_type()[i]));
    }

    return obs;

}

MountainCarData::state_t
MountainCarData::extract_state(obj_t gym_namespace, std::string result_name){

    auto obs =  boost::python::extract<MountainCarData::state_boost_python_t>(gym_namespace[result_name]);
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

}

}
