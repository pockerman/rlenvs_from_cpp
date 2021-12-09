#include "gymfcpp/mountain_car.h"
#include "gymfcpp/time_step.h"

namespace gymfcpp{


std::string MountainCar::name = "MountainCar";
std::string MountainCar::py_env_name = "py_mountain_car";
std::string MountainCar::py_step_result_name = "py_mountain_car_step_rslt";
std::string MountainCar::py_reset_result_name = "py_mountain_car_reset_rslt";

MountainCar::MountainCar(std::string version, obj_t gym_namespace, bool do_create)
    :
      EnvBase("MountainCar", version, MountainCar::py_env_name, gym_namespace)
{}

void
MountainCar::make(){

    if(this->is_created()){
        return;
    }

    std::string cpp_str = "import gym \n";
    cpp_str += "import numpy as np \n";
    cpp_str += MountainCar::py_env_name + " = gym.make('" + MountainCar::name + "-" + std::string(this->version()) + "').unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), this->gym_namespace());
    this->env() = boost::python::extract<boost::python::api::object>(this->gym_namespace()[MountainCar::py_env_name]);
    this->make_created();

}

MountainCar::time_step_t
MountainCar::reset(){

}


MountainCar::time_step_t
MountainCar::step(action_t action){

}

}
