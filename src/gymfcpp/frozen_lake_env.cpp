#include "gymfcpp/frozen_lake_env.h"
#include "gymfcpp/names_generator.h"
#include "gymfcpp/config.h"

#include <boost/python.hpp>

#include <string>
#include <any>
#include <unordered_map>
#include <iostream>
#include <random>

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

namespace gymfcpp{

discrete_state_space_frozen_lake<4>::item_type
discrete_state_space_frozen_lake<4>::sample(){

    std::uniform_int_distribution<> dist(0, 16 - 1);

    std::random_device rd;
    std::mt19937 gen(rd());

    return dist(gen);

}

discrete_state_space_frozen_lake<8>::item_type
discrete_state_space_frozen_lake<8>::sample(){

    std::uniform_int_distribution<> dist(0, 64 - 1);

    std::random_device rd;
    std::mt19937 gen(rd());

    return dist(gen);

}

template<uint_t side_size>
const std::string FrozenLakeData<side_size>::name = "FrozenLake";


template<uint_t side_size>
typename FrozenLakeData<side_size>::state_type
FrozenLakeData<side_size>::extract_state_from_reset(obj_t gym_namespace, std::string py_state_name, std::string result_name){

    std::string s = py_state_name + " = " + result_name + "\n";

    // reset the python environment
    boost::python::exec(s.c_str(), gym_namespace);

    // the observation
    auto observation =  boost::python::extract<uint_t>(gym_namespace[py_state_name]);

    return observation;
}

template<uint_t side_size>
typename FrozenLakeData<side_size>::state_type
FrozenLakeData<side_size>::extract_state_from_step(obj_t /*gym_namespace*/, std::string /*py_state_name*/,
                                                   std::string /*result_name*/){

#ifdef GYMFCPP_DEBUG
    assert(false && "FrozenLakeData<side_size>::extract_state_from_step should not be called");
#endif

   return 0;
}



template<uint_t side_size>
typename FrozenLakeData<side_size>::state_type
FrozenLakeData<side_size>::state_transform_from_boost(state_boost_python_type /*boost_type*/){

#ifdef GYMFCPP_DEBUG
    assert(false && "FrozenLakeData<side_size>::state_transform_from_boost should not be called");
#endif

    return 0;
}

template<uint_t side_size>
typename FrozenLakeData<side_size>::state_type
FrozenLakeData<side_size>::extract_state(obj_t /*gym_namespace*/, std::string /*result_name*/){

#ifdef GYMFCPP_DEBUG
    assert(false && "FrozenLakeData<side_size>::extract_state should not be called");
#endif

    return 0;
}

template<uint_t side_size>
FrozenLake<side_size>::FrozenLake(const std::string& version, obj_t main_namespace,
                       bool do_create,  bool is_slippery)
    :
      EnvMixin<FrozenLakeData<side_size>>(version, main_namespace),
      is_slippery_(is_slippery)
{
    this->py_env_name = get_py_env_name(FrozenLakeData<side_size>::name);
    this->py_reset_result_name = get_py_reset_rslt_name(FrozenLakeData<side_size>::name);
    this->py_step_result_name = get_py_step_rslt_name(FrozenLakeData<side_size>::name);
    this->py_state_name = get_py_state_name(FrozenLakeData<side_size>::name);

    if(do_create){
        make();
    }
}

template<uint_t side_size>
FrozenLake<side_size>::~FrozenLake(){
    close();
}


template<uint_t side_size>
void
FrozenLake<side_size>::make(){

    if(is_created){
        return;
    }

    std::string cpp_str = construct_python_string_();

    auto ignored = boost::python::exec(cpp_str.c_str(), gym_namespace);
    this->env = boost::python::extract<boost::python::api::object>(gym_namespace[this->py_env_name]);
    this->is_created = true;
}

template<uint_t side_size>
typename FrozenLake<side_size>::time_step_type
FrozenLake<side_size>::step(action_type action, bool query_extra){

#ifdef GYMFCPP_DEBUG
    assert(this->is_created && "Environment has not been created");
#endif

    if(this->current_state.last()){
        return reset();
    }

    std::string s = this->py_step_result_name + " = " + this->py_env_name +".step("+std::to_string(action)+")";

    // create an environment
    boost::python::exec(s.c_str(), this->gym_namespace);

    // the observation
    auto result =  boost::python::extract<boost::python::tuple>(this->gym_namespace[this->py_step_result_name]);

    auto observation = boost::python::extract<uint_t>(result()[0]);
    auto reward = boost::python::extract<real_t>(result()[1]);
    auto done = boost::python::extract<bool>(result()[2]);

    std::unordered_map<std::string, std::any> extra;

    if(query_extra){

        auto prob_dict = boost::python::extract<boost::python::dict>(result()[3]);
        auto prob = boost::python::extract<real_t>(prob_dict()["prob"]);
        extra["prob"] = std::any(prob());
    }

    this->current_state = TimeStep(done() ? TimeStepTp::LAST : TimeStepTp::MID, reward(), observation(), std::move(extra));
    return this->current_state;

}

template<uint_t side_size>
typename FrozenLake<side_size>::dynamics_t
FrozenLake<side_size>::p(uint_t sidx, uint_t aidx)const{

#ifdef GYMFCPP_DEBUG
    assert(this->is_created && "Environment has not been created");
#endif

    std::string s = "dynamics = " + FrozenLake::py_env_name + ".P["+std::to_string(sidx)+"]";
    s += "["+std::to_string(aidx)+"]";
    str_t exe_str = s.c_str();

    // get the dynamics
    boost::python::exec(exe_str, this->gym_namespace);
    auto dynamics_list = boost::python::extract<boost::python::list>(this->gym_namespace["dynamics"]);

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


template<uint_t side_size>
std::string
FrozenLake<side_size>::construct_python_string_()const noexcept{

    std::string cpp_str = "import gym \n";

    if(is_slippery()){

        if(map_type() == "4x4"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData<side_size>::name +"-" + version + "', map_name='4x4', is_slippery=True).unwrapped\n";
        }
        else if(map_type() == "8x8"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData<side_size>::name +"-" + version + "', map_name='8x8', is_slippery=True).unwrapped\n";
        }
    }
    else{

        if(map_type() == "4x4"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData<side_size>::name +"-" + version + "', map_name='4x4', is_slippery=False).unwrapped\n";
        }
        else if(map_type() == "8x8"){
            cpp_str += py_env_name + " = gym.make('" + FrozenLakeData<side_size>::name +"-" + version + "', map_name='8x8', is_slippery=False).unwrapped\n";
        }
    }


    return cpp_str;

}


template class FrozenLake<4>;
template class FrozenLake<8>;

}
