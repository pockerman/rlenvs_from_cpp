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

namespace rlenvs_cpp{



///
/// \brief The SerialVectorEnvWrapperConfig struct
///
struct SerialVectorEnvWrapperConfig
{
    std::string env_id{""};
    uint_t n_copies{2};
    uint_t seed{42};
};

namespace  {

template<typename EnvType>
class serial_vector_env_wrapper_base: private boost::noncopyable
{
public:

    ///
    /// \brief serial_vector_env_wrapper_base
    /// \param config
    /// \param p_namespace
    ///
    serial_vector_env_wrapper_base(SerialVectorEnvWrapperConfig config, obj_t p_namespace);

    ///
    ///
    ///
    ~serial_vector_env_wrapper_base();

    ///
    /// \brief close. Close down the environment
    ///
    void close();

    ///
    /// \brief n_copies
    /// \return
    ///
    uint_t n_copies()const noexcept{return config_.n_copies;}

protected:

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

template<typename EnvType>
serial_vector_env_wrapper_base<EnvType>::serial_vector_env_wrapper_base(SerialVectorEnvWrapperConfig config, obj_t p_namespace)
    :
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

template<typename EnvType>
serial_vector_env_wrapper_base<EnvType>::~serial_vector_env_wrapper_base(){

    close();
}

template<typename EnvType>
void
serial_vector_env_wrapper_base<EnvType>::close(){

    for(uint_t t=0; t < config_.n_copies; ++t){
        envs_[t]->close();
    }
}

}



template<typename EnvType>
class SerialVectorEnvWrapper: protected serial_vector_env_wrapper_base<EnvType>
{
public:

    typedef VectorTimeStep<typename EnvType::state_type> time_step_type;

    ///
    /// \brief VectorEnvWrapper
    /// \param n_copies
    /// \param version
    ///
    SerialVectorEnvWrapper(SerialVectorEnvWrapperConfig config, obj_t p_namespace);

    ///
    /// \brief ~VectorEnvWrapper
    ///
    ~SerialVectorEnvWrapper()=default;

    using serial_vector_env_wrapper_base<EnvType>::n_copies;
    using serial_vector_env_wrapper_base<EnvType>::close;

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

private:

    ///
    /// \brief current_state
    ///
    time_step_type current_state_;

};

template<typename EnvType>
SerialVectorEnvWrapper<EnvType>::SerialVectorEnvWrapper(SerialVectorEnvWrapperConfig config, obj_t p_namespace)
    :
   serial_vector_env_wrapper_base<EnvType>(config, p_namespace),
   current_state_()
{}

template<typename EnvType>
typename SerialVectorEnvWrapper<EnvType>::time_step_type
SerialVectorEnvWrapper<EnvType>::reset(){

#ifdef GYMFCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif

    current_state_.clear();
    current_state_.reserve(this->config_.n_copies);

    for(uint_t env=0; env<this->envs_.size(); ++env){
        auto time_step = this->envs_[env]->reset();
        current_state_.add_time_step(time_step);
    }

    return current_state_;
}

template<typename EnvType>
typename SerialVectorEnvWrapper<EnvType>::time_step_type
SerialVectorEnvWrapper<EnvType>::step(const std::vector<typename EnvType::action_type>& actions){


#ifdef GYMFCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
    assert(actions.size() == this->envs_.size() && "Invalid number of actions. Number of actions does not equal number of environments");
#endif

    current_state_.clear();
    current_state_.reserve(this->config_.n_copies);

    for(uint_t env=0; env<this->envs_.size(); ++env){
        auto time_step = this->envs_[env]->step(actions[env]);
        current_state_.add_time_step(time_step);

        // if the current environment finished
        // reset it
        if(time_step.last()){
            this->envs_[env]->reset();
        }
    }

    return current_state_;

}


template<typename EnvType>
void
SerialVectorEnvWrapper<EnvType>::make(){

    if(this->is_created_){
        return;
    }

    this->envs_.reserve(this->config_.n_copies);
    for(uint_t env=0; env < this->config_.n_copies; ++env){
        this->envs_.push_back(std::make_shared<EnvType>(this->config_.env_id, this->p_namespace_, false));
        this->envs_.back()->make();
    }

    // reserve space for time steps
    current_state_.reserve(this->config_.n_copies);
    this->is_created_ = true;
}


}

#endif // VECTOR_ENV_WRAPPER_H
