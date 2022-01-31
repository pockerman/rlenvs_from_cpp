#include "gymfcpp/cliff_world_env.h"
#include "gymfcpp/config.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <boost/python/numpy.hpp>

namespace gymfcpp{

std::string CliffWorld::name = "CliffWalking";
std::string CliffWorld::py_env_name = "cliffwalking_env";
std::string CliffWorld::py_obs_name = "cliff_walk_obs";
std::string CliffWorld::py_step_result_name = "cliff_walk_step_result";
std::string CliffWorld::py_dynamics_name = "cliff_walk_dynamics";

CliffWorld::CliffWorld(std::string version, obj_t gym_namespace, bool do_create)
    :
      v_(version),
      is_created_(false),
      gym_namespace_(gym_namespace),
      world_(),
      current_state_(),
      cached_n_actions_(INVALID_UINT),
      cached_n_states_(INVALID_UINT)
{}

CliffWorld::~CliffWorld(){
    close();
}

void
CliffWorld::make(){

    std::string cpp_str = "import gym \n";
    cpp_str += CliffWorld::py_env_name + " = gym.make('" + CliffWorld::name +"-" + v_ + "')\n";
    cpp_str += CliffWorld::py_env_name + " = " + CliffWorld::py_env_name + ".unwrapped";

    // create an environment
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace_);
    world_ = boost::python::extract<boost::python::api::object>(gym_namespace_[CliffWorld::py_env_name]);
    is_created_ = true;
}


uint_t
CliffWorld::n_states()const{
#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    if(cached_n_states_ != INVALID_UINT){
        return cached_n_states_;
    }

    namespace np = boost::python::numpy;

    //auto world_dict = boost::python::extract<boost::python::dict>(world_.attr("__dict__"));
    //auto observation_space = boost::python::extract<boost::python::api::object>(world_dict()["observation_space"]);

    // workaround to avoid the problem with np.int64
    std::string cpp_str ="cliff_world_n_states = " + CliffWorld::py_env_name + ".observation_space.n.item()";
    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace_);
    cached_n_states_ = boost::python::extract<uint_t>(gym_namespace_["cliff_world_n_states"]);
    return cached_n_states_;
}

uint_t
CliffWorld::n_actions()const{

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    if(cached_n_actions_ != INVALID_UINT){
        return cached_n_actions_;
    }

    auto world_dict = boost::python::extract<boost::python::dict>(world_.attr("__dict__"));
    auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);

    //std::string cpp_str ="cliff_world_n_actions = " + CliffWorld::py_env_name + ".action_space.n.item()";
    //auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace_);

    cached_n_actions_ = boost::python::extract<uint64_t>(action_space().attr("__dict__")["n"]);
    return cached_n_actions_;
}

CliffWorld::time_step_t
CliffWorld::reset(){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    auto cpp_str = std::string(CliffWorld::py_obs_name + " = ");
    cpp_str += CliffWorld::py_env_name + ".reset()";

    // reset the python environment
    boost::python::exec(cpp_str.c_str(), gym_namespace_);

    // the observation
    auto observation =  boost::python::extract<uint_t>(gym_namespace_[CliffWorld::py_obs_name]);

    current_state_ = time_step_t(TimeStepTp::FIRST, 0.0, observation);
    return current_state_;

}


CliffWorld::time_step_t
CliffWorld::step(action_t action, bool query_extra){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    if(current_state_.last()){
        return reset();
    }

    std::string s = CliffWorld::py_step_result_name + " = " + CliffWorld::py_env_name +".step("+std::to_string(action)+")";

    // create an environment
    boost::python::exec(s.c_str(), gym_namespace_);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace_[CliffWorld::py_step_result_name]);

    auto observation = boost::python::extract<uint_t>(result()[0]);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    if(query_extra){

        auto prob_dict = boost::python::extract<boost::python::dict>(result()[3]);
        auto prob = boost::python::extract<real_t>(prob_dict()["prob"]);
        extra["prob"] = std::any(prob());
    }

    current_state_ = time_step_t(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), observation(), std::move(extra));
    return current_state_;

}

CliffWorld::dynamics_t
CliffWorld::p(uint_t sidx, uint_t aidx)const{

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    std::string s = CliffWorld::py_dynamics_name +"_dummy = " + CliffWorld::py_env_name + ".P["+std::to_string(sidx)+"]";
    s += "["+std::to_string(aidx)+"] \n";

    // TODO: This does not seem to work
    // for this environment (it does for FrozenLake!!)
    // check this article about adding a converter
    // https://www.py4u.net/discuss/84152
    // The error we get is TypeError: No registered converter was able to produce a C++ rvalue of type unsigned long from this Python object of type numpy.int64
    s += CliffWorld::py_dynamics_name + "= [(item[0], item[1].item(), item[2], item[3]) for item in " + CliffWorld::py_dynamics_name +"_dummy" + "]";

    // get the dynamics
    boost::python::exec(s.c_str(), gym_namespace_);
    auto dynamics_list = boost::python::extract<boost::python::list>(gym_namespace_[CliffWorld::py_dynamics_name.c_str()]);

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
CliffWorld::render(std::string mode){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    auto str = CliffWorld::py_env_name + ".render(mode=" + mode + ")\n";
    boost::python::exec(str.c_str(), gym_namespace_);

}

void
CliffWorld::close(){

    if(!is_created_){
        return;
    }

    auto str = CliffWorld::py_env_name + ".close()\n";
    boost::python::exec(str.c_str(), gym_namespace_);

}



}
