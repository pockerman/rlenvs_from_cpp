#ifndef STABLE_BASELINE_VECTOR_ENV_WRAPPER_H
#define STABLE_BASELINE_VECTOR_ENV_WRAPPER_H

/**
  * Vector environment wrapper. This class provides a C++
  * interface to OpenAI-Gym vectorized environment.
  * For more information: https://www.gymlibrary.ml/content/vector_api/
  */

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/vector_time_step.h"


#include <boost/noncopyable.hpp>
#include <boost/python.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <memory>

namespace rlenvs{

using namespace gymfcpp;


struct SerialVectorEnvWrapperConfig
{
    std::string env_id{""};
    uint_t n_copies{2};
    uint_t seed{42};
};


///
///
///
template<typename EnvType, typename StateAdaptor=void>
class SerialVectorEnvWrapper: private boost::noncopyable
{
public:

    typedef VectorTimeStep<typename EnvType::state_type, StateAdaptor> time_step_type;

    ///
    /// \brief VectorEnvWrapper
    /// \param n_copies
    /// \param version
    ///
    SerialVectorEnvWrapper(SerialVectorEnvWrapperConfig config, obj_t p_namespace);

    ///
    /// \brief ~VectorEnvWrapper
    ///
    ~SerialVectorEnvWrapper();

    ///
    /// \brief make
    ///
    void make();

    ///
    ///
    ///
    time_step_type reset();

    ///
    ///
    ///
    time_step_type step(const std::vector<typename EnvType::action_type>& actions);

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
    /// \brief envs_
    ///
    std::vector<std::shared_ptr<EnvType>> envs_;

    ///
    /// \brief config_
    ///
    SerialVectorEnvWrapperConfig config_;

    ///
    /// \brief is_created_
    ///
    bool is_created_;

};


template<typename EnvType, typename StateAdaptor>
SerialVectorEnvWrapper<EnvType, StateAdaptor>::SerialVectorEnvWrapper(SerialVectorEnvWrapperConfig config, obj_t p_namespace)
    :
   current_state_(),
   p_namespace_(p_namespace),
   envs_(),
   config_(config),
   is_created_(false)
{

#ifdef GYMFCPP_DEBUG
    assert(config_.n_copies != 0 && "Invalid number of copies. n_copies should be > 0");
    assert(config_.env_id != "" && "Invalid environment id");
#endif

}

template<typename EnvType, typename StateAdaptor>
SerialVectorEnvWrapper<EnvType, StateAdaptor>::~SerialVectorEnvWrapper(){

    close();
}

template<typename EnvType, typename StateAdaptor>
void
SerialVectorEnvWrapper<EnvType, StateAdaptor>::close(){

    for(uint_t t=0; t < config_.n_copies; ++t){
        envs_[t]->close();
    }
}

template<typename EnvType, typename StateAdaptor>
typename SerialVectorEnvWrapper<EnvType, StateAdaptor>::time_step_type
SerialVectorEnvWrapper<EnvType, StateAdaptor>::reset(){

#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
#endif

    current_state_.clear();
    current_state_.reserve(config_.n_copies);

    for(uint_t env=0; env<envs_.size(); ++env){
        auto time_step = envs_[env]->reset();
        current_state_.add_time_step(time_step);
    }

    return current_state_;
}

template<typename EnvType, typename StateAdaptor>
typename SerialVectorEnvWrapper<EnvType, StateAdaptor>::time_step_type
SerialVectorEnvWrapper<EnvType, StateAdaptor>::step(const std::vector<typename EnvType::action_type>& actions){


#ifdef GYMFCPP_DEBUG
    assert(is_created_ && "Environment has not been created");
    assert(actions.size() == envs_.size() && "Invalid number of actions. Number of actions does not equal number of environments");
#endif

    current_state_.clear();
    current_state_.reserve(config_.n_copies);

    for(uint_t env=0; env<envs_.size(); ++env){
        auto time_step = envs_[env]->step(actions[env]);
        current_state_.add_time_step(time_step);
    }

    return current_state_;

}

template<typename EnvType, typename StateAdaptor>
void
SerialVectorEnvWrapper<EnvType, StateAdaptor>::make(){

    if(is_created_){
        return;
    }

    envs_.reserve(config_.n_copies);
    for(uint_t env=0; env < config_.n_copies; ++env){
        envs_.push_back(std::make_shared<EnvType>(config_.env_id, p_namespace_, false));
        envs_.back()->make();
    }

    // reserve space for time steps
    current_state_.reserve(config_.n_copies);
    is_created_ = true;
}

}

#endif // VECTOR_ENV_WRAPPER_H
