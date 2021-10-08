#include "gymfcpp/cart_pole.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/config.h"

namespace gymfcpp{

// static data
std::string CartPole::name = "CartPole";
std::string CartPole::py_env_name = "py_cart_pole";
std::string CartPole::py_step_result_name = "py_cart_pole_step_rslt";


CartPole::CartPole(const std::string& version, obj_t gym_namespace, bool do_create)
    :
      data_(),
      n_actions_(INVALID_UINT)
{
    data_.v = version;
    data_.gym_namespace = gym_namespace;
    data_.is_created = false;

    if(do_create){
        make();
    }

}


void
CartPole::make(){

    if(data_.is_created){
        return;
    }

    std::string cpp_str = "import gym \n";
    cpp_str += CartPole::py_env_name + " = gym.make('" + CartPole::name +"-" + data_.v + "')\n";
    cpp_str += CartPole::py_env_name + " = " + CartPole::py_env_name + ".unwrapped";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), data_.gym_namespace);
    data_.world = boost::python::extract<boost::python::api::object>(data_.gym_namespace[CartPole::py_env_name]);
    data_.is_created = true;

}


uint_t
CartPole::n_actions()const{

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    if(n_actions_ == INVALID_UINT){

        auto world_dict = boost::python::extract<boost::python::dict>(data_.world.attr("__dict__"));
        auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);
        n_actions_ = boost::python::extract<uint64_t>(action_space().attr("__dict__")["n"]);

    }

    return n_actions_;
}

TimeStep
CartPole::reset(){

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    auto cpp_str = std::string(CartPole::py_env_name + " = ");
    cpp_str += CartPole::py_env_name + ".reset()";

    // reset the python environment
    boost::python::exec(cpp_str.c_str(), data_.gym_namespace);

    // the observation
    auto observation =  boost::python::extract<boost::python::list>(data_.gym_namespace[CartPole::py_env_name]);

    data_.current_state = TimeStep(TimeStepTp::FIRST, 0.0, observation);
    return data_.current_state;

}


TimeStep
CartPole::step(action_t action){

#ifdef GYMFCPP_DEBUG
    assert(data_.is_created && "Environment has not been created");
#endif

    if(data_.current_state.last()){
        return reset();
    }

    std::string s = CartPole::py_step_result_name + " = " + CartPole::py_env_name +".step("+std::to_string(action)+")";

    // create an environment
    boost::python::exec(s.c_str(), data_.gym_namespace);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(data_.gym_namespace[CartPole::py_step_result_name]);

    auto observation = boost::python::extract<uint_t>(result()[0]);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    data_.current_state = TimeStep(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), observation(), std::move(extra));
    return data_.current_state;

}

}
