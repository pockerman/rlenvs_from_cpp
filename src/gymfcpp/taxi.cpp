#include "gymfcpp/taxi.h"
#include "gymfcpp/names_generator.h"

namespace gymfcpp
{

std::string TaxiData::name = "Taxi";
std::string TaxiData::py_env_name = get_py_env_name(TaxiData::name); //"py_mountain_car";
std::string TaxiData::py_step_result_name = get_py_step_rslt_name(TaxiData::name); //"py_mountain_car_step_rslt";
std::string TaxiData::py_reset_result_name = get_py_reset_rslt_name(TaxiData::name); //"py_mountain_car_reset_rslt";
std::string TaxiData::py_state_name = get_py_state_name(TaxiData::name); //"py_mountain_car_state";


TaxiData::state_t
TaxiData::extract_state(obj_t gym_namespace, std::string result_name){

    std::string s;
    if(result_name == TaxiData::py_reset_result_name){
        s = TaxiData::py_state_name +   " = " +  result_name + "\n";
    }
    else if(result_name == TaxiData::py_step_result_name){

        s = TaxiData::py_state_name + " = " + result_name + "[0]\n";
    }
#ifdef GYMFCPP_DEBUG
    else{
        assert(false && result_name + "not " + TaxiData::py_reset_result_name + " or " + TaxiData::py_step_result_name);
    }
#endif

    boost::python::exec(s.c_str(), gym_namespace);
    auto obs =  boost::python::extract<uint_t>(gym_namespace[TaxiData::py_state_name]);
    return obs;

}

Taxi::Taxi(std::string version, obj_t main_namespace, bool do_create)
    :
      EnvMixin<TaxiData>(version, main_namespace)
{
    if(do_create){
        make();
    }
}

void
Taxi::make(){

    if(is_created){
        return;
    }

    std::string cpp_str = "import gym \n";
    //cpp_str += "import numpy as np \n";
    cpp_str += TaxiData::py_env_name + " = gym.make('" + TaxiData::name + "-" + version + "').unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    env = boost::python::extract<boost::python::api::object>(gym_namespace[TaxiData::py_env_name]);
    is_created = true;
}


Taxi::time_step_t
Taxi::step(action_t action){

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
    assert(action < n_actions() && "Invalid action index");
#endif

    if(current_state.last()){
        return reset();
    }

    std::string s = TaxiData::py_step_result_name + " = " + TaxiData::py_env_name +".step("+std::to_string(action)+")\n";
    //s += TaxiData::py_step_result_name + " = (" + TaxiData::py_step_result_name + "[0].tolist(),";
    //s += "float(" + TaxiData::py_step_result_name + "[1]),";
    //s += TaxiData::py_step_result_name + "[2])\n";

    boost::python::exec(s.c_str(), gym_namespace);

    auto obs = TaxiData::extract_state(gym_namespace, TaxiData::py_step_result_name);

    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace[TaxiData::py_step_result_name]);

    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    current_state = time_step_t(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), obs, std::move(extra));
    return current_state;
}


}
