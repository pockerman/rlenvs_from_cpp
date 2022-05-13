#ifndef STABLE_BASELINE_VECTOR_ENV_WRAPPER_H
#define STABLE_BASELINE_VECTOR_ENV_WRAPPER_H

/**
  * Vector environment wrapper. This class provides a C++
  * interface to Stable baselines vector environments. This
  * approach is followed for the following reasons
  *
  * - Most of the environments use Boost.Python for their implementation
  * - Boost.Python is not thread safe
  * - Using MPI to spawn processes or any other mechanism currently is deem unneccessary
  */

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/vector_time_step.h"


#include <boost/noncopyable.hpp>
#include <boost/python.hpp>
#include <string>
#include <iostream>

namespace rlenvs
{
// still we may want to use some utilities
using namespace gymfcpp;

namespace  {

std::string get_make_subproc_vec_env_(std::string env_name, std::string env_id, uint_t n_copies, uint_t seed, std::string start_method){


    // Here we use the "fork" method for launching the processes, more information is available in the doc
    // This is equivalent to make_vec_env(env_id, n_envs=n_procs, vec_env_cls=SubprocVecEnv, vec_env_kwargs=dict(start_method='fork'))
    //
    std::string cmd =  env_name + "= SubprocVecEnv([make_env(" + env_id + " , rank=i, seed= ";
    cmd +=  std::to_string(seed) + ") for i in range(";
    cmd +=  std::to_string(n_copies) + ")], start_method=" +start_method + ") \n"; //'fork')"
    return cmd;
}

std::string get_make_envs_(){

    std::string cmd= "def make_env(env_id, rank, seed=0): \n";
    cmd += "\tdef _init():\n";
    cmd += "\t\tenv = gym.make(env_id); env.seed(seed + rank); retun env\n";
    cmd += "\tset_global_seeds(seed); return _init\n";

    return cmd;
}

}

struct StableBaselineVectorEnvWrapperConfig
{
    std::string env_id{""};
    std::string start_method{"fork"};
    std::string py_env_name{"stable_baseline_vec_train_env"};
    uint_t n_copies{2};
    uint_t seed{42};
};


template<typename TimeStepType, typename StateAdaptor=void>
class StableBaselineVectorEnvWrapper: private boost::noncopyable
{
public:

    typedef VectorTimeStep<TimeStepType, StateAdaptor> time_step_type;

    ///
    /// \brief VectorEnvWrapper
    /// \param n_copies
    /// \param version
    ///
    StableBaselineVectorEnvWrapper(StableBaselineVectorEnvWrapperConfig config, obj_t p_namespace);

    ///
    /// \brief ~VectorEnvWrapper
    ///
    ~StableBaselineVectorEnvWrapper();

    ///
    /// \brief make
    ///
    void make();

    ///
    ///
    ///
    time_step_type reset();

    ///
    /// \brief close. Close down the environment
    ///
    void close();

    ///
    /// \brief n_copies
    /// \return
    ///
    uint_t n_copies()const noexcept{return config_.n_copies;}

private:

    ///
    /// \brief current_state
    ///
    time_step_type current_state_;

    ///
    /// \brief p_namespace_ The main Python namespace
    ///
    obj_t p_namespace_;

    ///
    /// \brief env
    ///
    obj_t env_;

    ///
    /// \brief config_
    ///
    StableBaselineVectorEnvWrapperConfig config_;

    ///
    /// \brief is_created_
    ///
    bool is_created_;

};


template<typename TimeStepType, typename StateAdaptor>
StableBaselineVectorEnvWrapper<TimeStepType, StateAdaptor>::StableBaselineVectorEnvWrapper(StableBaselineVectorEnvWrapperConfig config, obj_t p_namespace)
    :
   current_state_(),
   p_namespace_(p_namespace),
   env_(),
   config_(config),
   is_created_(false)
{

#ifdef GYMFCPP_DEBUG
    assert(config_.n_copies != 0 && "Invalid number of copies. n_copies should be > 0");
    assert(config_.env_id != "" && "Invalid environment id");
#endif

}

template<typename TimeStepType, typename StateAdaptor>
StableBaselineVectorEnvWrapper<TimeStepType, StateAdaptor>::~StableBaselineVectorEnvWrapper(){

    close();
}

template<typename TimeStepType, typename StateAdaptor>
void
StableBaselineVectorEnvWrapper<TimeStepType, StateAdaptor>::close(){
}

template<typename TimeStepType, typename StateAdaptor>
typename StableBaselineVectorEnvWrapper<TimeStepType, StateAdaptor>::time_step_type
StableBaselineVectorEnvWrapper<TimeStepType, StateAdaptor>::reset(){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif


    return current_state_;
}

template<typename TimeStepType, typename StateAdaptor>
void
StableBaselineVectorEnvWrapper<TimeStepType, StateAdaptor>::make(){

    if(is_created_){
        return;
    }

    std::string cpp_str = "import gym \n";

    // import the stable baselines
    cpp_str += "from stable_baselines.common.vec_env import SubprocVecEnv \n";
    cpp_str += "from stable_baselines.common import set_global_seeds\n";

    // import the neccessary modules
    auto ignored = boost::python::exec(cpp_str.c_str(), p_namespace_);

    std::string get_make_envs_str = get_make_envs_();
    boost::python::exec(get_make_envs_str.c_str(), p_namespace_);

    std::string env_str = get_make_subproc_vec_env_(config_.py_env_name,
                                                    config_.env_id, config_. n_copies,
                                                    config_.seed, config_.start_method);

    boost::python::exec(env_str.c_str(), p_namespace_);
    env_ = boost::python::extract<boost::python::api::object>(p_namespace_[config_.py_env_name]);
    is_created_ = false;

}



}

#endif // VECTOR_ENV_WRAPPER_H
