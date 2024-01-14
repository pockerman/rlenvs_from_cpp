#include "rlenvs/envs/gymnasium/taxi_env.h"
#include "rlenvs/names_generator.h"
#include "rlenvs/rlenvs_consts.h"

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium{

std::string TaxiData::name = "Taxi";


TaxiData::state_type
TaxiData::extract_state_from_reset(obj_t gym_namespace, std::string py_state_name, std::string result_name){

     std:: string s = py_state_name +   " = " +  result_name + "\n";

     boost::python::exec(s.c_str(), gym_namespace);
     auto obs =  boost::python::extract<uint_t>(gym_namespace[py_state_name]);
     return obs;
}

TaxiData::state_type
TaxiData::extract_state_from_step(obj_t gym_namespace, std::string py_state_name, std::string result_name){

     std:: string s = py_state_name +   " = " +  result_name + "[0]\n";

     boost::python::exec(s.c_str(), gym_namespace);
     auto obs =  boost::python::extract<uint_t>(gym_namespace[py_state_name]);
     return obs;
}

Taxi::Taxi(std::string version, obj_t main_namespace, bool do_create)
    :
      EnvMixin<TaxiData>(version, main_namespace)
{
    this->EnvMixin<TaxiData>::py_env_name = get_py_env_name(TaxiData::name);
    this->EnvMixin<TaxiData>::py_reset_result_name = get_py_reset_rslt_name(TaxiData::name);
    this->EnvMixin<TaxiData>::py_step_result_name = get_py_step_rslt_name(TaxiData::name);
    this->EnvMixin<TaxiData>::py_state_name = get_py_state_name(TaxiData::name);

    if(do_create){
        make();
    }
}

Taxi::Taxi(std::string version, obj_t main_namespace, std::map<std::string, std::string>&& names)
    :
    EnvMixin<TaxiData>(version, main_namespace)
{
    this->EnvMixin<TaxiData>::py_env_name = names[PY_ENV_NAME];
    this->EnvMixin<TaxiData>::py_reset_result_name = names[PY_RESET_ENV_RESULT_NAME];
    this->EnvMixin<TaxiData>::py_step_result_name = names[PY_STEP_ENV_RESULT_NAME];
    this->EnvMixin<TaxiData>::py_state_name = names[PY_STATE_NAME];
}

Taxi::Taxi(const Taxi& other)
    :
       EnvMixin<TaxiData>(other.version, other.gym_namespace)
{

    this->py_env_name = other.py_env_name;
    this->py_reset_result_name = other.py_reset_result_name;
    this->py_step_result_name = other.py_step_result_name;
    this->py_state_name = other.py_state_name;
}


void
Taxi::make(){

    if(is_created){
        return;
    }

    std::string cpp_str = "import gym \n";
    cpp_str += this->EnvMixin<TaxiData>::py_env_name + " = gym.make('" + TaxiData::name + "-" + version + "').unwrapped\n";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    env = boost::python::extract<boost::python::api::object>(gym_namespace[this->EnvMixin<TaxiData>::py_env_name]);
    is_created = true;
}


Taxi::time_step_type
Taxi::step(action_type action){

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
    assert(action < n_actions() && "Invalid action index");
#endif

    if(current_state.last()){
        return reset();
    }

    std::string s = this->EnvMixin<TaxiData>::py_step_result_name  + " = "
            + this->EnvMixin<TaxiData>::py_env_name +".step("+std::to_string(action)+")\n";

    boost::python::exec(s.c_str(), gym_namespace);

    auto obs = TaxiData::extract_state_from_step(gym_namespace, this->py_state_name, this->py_step_result_name);

    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace[this->py_step_result_name]);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    current_state = time_step_type(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), obs, 1.0, std::move(extra));
    return current_state;
}

Taxi
Taxi::copy(std::map<std::string, std::string>&& names)const{

    // before copying we need to copy in the Python
    // interpreter
    std::string str = "import copy \n";
    str += names[PY_ENV_NAME] + " = copy.copy(" + this->py_env_name + ")\n";

    boost::python::exec(str.c_str(), this->gym_namespace);
    return Taxi(this->version, this->gym_namespace, std::move(names));
}

}
}
}
