#include "rlenvs/envs/gymnasium/cliff_world_env.h"
#include "rlenvs/names_generator.h"
#include "rlenvs/rlenvscpp_config.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <boost/python/numpy.hpp>

namespace rlenvs_cpp{
namespace envs {
namespace gymnasium{

const std::string CliffWorldData::name = "CliffWalking";


CliffWorldData::state_type
CliffWorldData::extract_state_from_reset(obj_t gym_namespace, std::string py_state_name, std::string result_name){

    std::string s = py_state_name + " = " + result_name + "\n";

    // reset the python environment
    boost::python::exec(s.c_str(), gym_namespace);

    // the observation
    auto observation =  boost::python::extract<uint_t>(gym_namespace[py_state_name]);

    return observation;
}

CliffWorld::CliffWorld(std::string version, obj_t main_namespace, bool do_create)
    :
      EnvMixin<CliffWorldData>(version, main_namespace),
      py_dynamics_name("cliff_walk_dynamics")
{
    this->py_env_name = get_py_env_name(CliffWorldData::name);
    this->py_reset_result_name = get_py_reset_rslt_name(CliffWorldData::name);
    this->py_step_result_name = get_py_step_rslt_name(CliffWorldData::name);
    this->py_state_name = get_py_state_name(CliffWorldData::name);


    if(do_create){
        make();
    }
}

CliffWorld::~CliffWorld(){
    close();
}

void
CliffWorld::make(){

    std::string cpp_str = "import gym \n";
    cpp_str += CliffWorld::py_env_name + " = gym.make('" + CliffWorldData::name +"-" + this->version + "').unwrapped\n";
    //cpp_str += CliffWorld::py_env_name + " = " + CliffWorld::py_env_name + ".unwrapped";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), this->gym_namespace);
    this->env  = boost::python::extract<boost::python::api::object>(this->gym_namespace[this->py_env_name]);
    this->is_created = true;
}

CliffWorld::time_step_type
CliffWorld::step(action_type action, bool query_extra){

#ifdef GYMFCPP_DEBUG
    assert(this->is_created && "Environment has not been created");
#endif

    if(this->current_state.last()){
        return reset();
    }


    std::string s = this->py_step_result_name + " = " + this->py_env_name +".step("+std::to_string(action)+")";

    // create an environment
    boost::python::exec(s.c_str(), gym_namespace);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace[this->py_step_result_name]);

    auto observation = boost::python::extract<uint_t>(result()[0]);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    if(query_extra){

        auto prob_dict = boost::python::extract<boost::python::dict>(result()[3]);
        auto prob = boost::python::extract<real_t>(prob_dict()["prob"]);
        extra["prob"] = std::any(prob());
    }

    this->current_state = time_step_type(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), observation(), 1.0, std::move(extra));
    return current_state;

}

CliffWorld::dynamics_type
CliffWorld::p(uint_t sidx, uint_t aidx)const{

#ifdef GYMFCPP_DEBUG
    assert(this->is_created && "Environment has not been created");
#endif


    std::string s = this->py_dynamics_name +"_dummy = " + CliffWorld::py_env_name + ".P["+std::to_string(sidx)+"]";
    s += "["+std::to_string(aidx)+"] \n";

    // TODO: This does not seem to work
    // for this environment (it does for FrozenLake!!)
    // check this article about adding a converter
    // https://www.py4u.net/discuss/84152
    // The error we get is TypeError: No registered converter was able to produce a C++ rvalue of type unsigned long from this Python object of type numpy.int64
    s += this->py_dynamics_name + "= [(item[0], item[1].item(), item[2], item[3]) for item in " + this->py_dynamics_name +"_dummy" + "]";

    // get the dynamics
    boost::python::exec(s.c_str(), gym_namespace);
    auto dynamics_list = boost::python::extract<boost::python::list>(gym_namespace[this->py_dynamics_name.c_str()]);

    dynamics_type dyn;
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

}
}
}
