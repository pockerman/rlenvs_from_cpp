#include "gymfcpp/black_jack_env.h"
#include "gymfcpp/gymfcpp_config.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif


namespace rlenvs_cpp{
namespace gymfcpp
{

std::string BlackJack::name = "Blackjack";
std::string BlackJack::py_env_name = "black_jack_env";

BlackJack::BlackJack(std::string version, obj_t gym_namespace, bool do_create, bool natural)
    :
     v_(version),
     is_created_(false),
     gym_namespace_(gym_namespace),
     world_(),
     current_state_(),
     cached_n_actions_(INVALID_UINT),
     cached_obs_space_()
{
    if(do_create){
        make(natural);
    }
}

BlackJack::~BlackJack(){
    close();
}

void
BlackJack::make(bool natural){

    std::string cpp_str = "import gym \n";

    if(!natural){
        cpp_str += BlackJack::py_env_name + " = gym.make('Blackjack" + "-" + v_ + "',natural=False) \n";
    }
    else{
        cpp_str += BlackJack::py_env_name + " = gym.make('Blackjack" + "-" + v_ + "',natural=True) \n";
    }

    cpp_str += BlackJack::py_env_name + " = " + BlackJack::py_env_name + ".unwrapped";

    str_t python_str = cpp_str.c_str();

    // create an environment
    auto ignored = boost::python::exec(python_str, gym_namespace_);
    world_ = boost::python::extract<boost::python::api::object>(gym_namespace_[BlackJack::py_env_name.c_str()]);
    is_created_ = true;
}

uint_t
BlackJack::n_actions()const{

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    if(cached_n_actions_ != INVALID_UINT){
        return cached_n_actions_;
    }

    auto world_dict = boost::python::extract<boost::python::dict>(world_.attr("__dict__"));
    auto action_space = boost::python::extract<boost::python::api::object>(world_dict()["action_space"]);
    cached_n_actions_ = boost::python::extract<uint_t>(action_space().attr("__dict__")["n"]);
    return cached_n_actions_;
}

std::vector<uint_t>
BlackJack::observation_space()const{

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    if(!cached_obs_space_.empty()){
        return cached_obs_space_;
    }

    auto world_dict = boost::python::extract<boost::python::dict>(world_.attr("__dict__"));
    auto observation_space = boost::python::extract<boost::python::api::object>(world_dict()["observation_space"]);

    auto dict = boost::python::extract<boost::python::dict>(observation_space().attr("__dict__"));
    auto spaces = boost::python::extract<boost::python::tuple>(dict()["spaces"]);

    auto s1 = boost::python::extract<uint_t>(spaces()[0].attr("__dict__")["n"]);
    auto s2 = boost::python::extract<uint_t>(spaces()[1].attr("__dict__")["n"]);
    auto s3 = boost::python::extract<uint_t>(spaces()[2].attr("__dict__")["n"]);

    cached_obs_space_.resize(3);
    cached_obs_space_[0] = s1();
    cached_obs_space_[1] = s2();
    cached_obs_space_[2] = s3();
    return cached_obs_space_;

}

BlackJack::time_step_t
BlackJack::reset(){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    // reset the python environment
    std::string cpp_str = "observation=" + BlackJack::py_env_name + ".reset()";
    boost::python::exec(cpp_str.c_str(), gym_namespace_);

    // the observation
    auto observation =  boost::python::extract<boost::python::tuple>(gym_namespace_["observation"]);

    auto obs = boost::python::extract<uint_t>(observation()[0]);
    auto reward = boost::python::extract<real_t>(observation()[1]);

    current_state_ = TimeStep(TimeStepTp::FIRST, reward, obs());
    return current_state_;

}


BlackJack::time_step_t
BlackJack::step(action_t action){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");

    if(cached_n_actions_ != INVALID_UINT){
        assert(action < cached_n_actions_ && "Invalid action index");
    }
#endif

    if(current_state_.last()){
        return reset();
    }

    std::string s = "result = " + BlackJack::py_env_name +".step("+std::to_string(action)+")";
    str_t exe_str = s.c_str();

    // execute the step into the python
    boost::python::exec(exe_str, gym_namespace_);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(gym_namespace_["result"]);

    auto observation = boost::python::extract<boost::python::tuple>(result()[0]);

    auto obs = boost::python::extract<uint_t>(observation()[0]);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    auto dealers = boost::python::extract<uint_t>(observation()[1]);
    extra["dealers"] = std::any(dealers());

    auto usable = boost::python::extract<bool>(observation()[2]);
    extra["usable_ace"] = std::any(usable());

    current_state_ = time_step_t(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), obs(), std::move(extra));
    return current_state_;

}

void
BlackJack::render(std::string mode){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    auto str = "screen = " + BlackJack::py_env_name + ".render(mode=" + mode + ")\n";
    boost::python::exec(str.c_str(), gym_namespace_);
}

void
BlackJack::close(){

    if(!is_created_){
        return;
    }

    auto str = BlackJack::py_env_name + ".close()\n";
    boost::python::exec(str.c_str(), gym_namespace_);


}

}
}
