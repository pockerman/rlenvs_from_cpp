#ifndef VECTOR_TIME_STEP_H
#define VECTOR_TIME_STEP_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"

#include <vector>

namespace rlenvs{

using namespace gymfcpp;

template<typename StateType>
class VectorTimeStep
{

public:

    VectorTimeStep()=default;

    ///
    /// \brief reserve
    /// \param ncopies
    ///
    void reserve(uint_t ncopies){time_steps_.reserve(ncopies);}

    ///
    /// \brief clear
    ///
    void clear()noexcept{time_steps_.clear();}

    ///
    /// \brief add_time_step
    /// \param step
    ///
    void add_time_step(const TimeStep<StateType>& step);

    ///
    /// \brief size
    /// \return
    ///
    uint_t size()const noexcept{return time_steps_.size();}

    ///
    /// \brief empty
    /// \return
    ///
    bool empty()const noexcept{return time_steps_.empty();}

    ///
    ///
    ///
    template<typename AdaptorType>
    typename AdaptorType::value_type stack_states()const;

    ///
    ///
    ///
    template<typename AdaptorType>
    typename AdaptorType::value_type stack_rewards()const;

    ///
    /// \brief stack_time_step_types
    /// \return
    ///
    std::vector<TimeStepTp> stack_time_step_types()const;

private:

    ///
    /// \brief time_steps_
    ///
    std::vector<TimeStep<StateType>> time_steps_;

};

template<typename StateType>
void
VectorTimeStep<StateType>::add_time_step(const TimeStep<StateType>& step){
    time_steps_.push_back(step);
}

template<typename StateType>
template<typename AdaptorType>
typename AdaptorType::value_type
VectorTimeStep<StateType>::stack_states()const{

    std::vector<typename AdaptorType::value_type> states;
    states.reserve(time_steps_.size());

    AdaptorType adaptor;

    std::for_each(time_steps_.begin(), time_steps_.end(),
                  [&](const auto& step){

        states.push_back(adaptor(step.observation()));

    });

    return adaptor.stack(states);
}

template<typename StateType>
template<typename AdaptorType>
typename AdaptorType::value_type
VectorTimeStep<StateType>::stack_rewards()const{

    std::vector<real_t> rewards;
    rewards.reserve(time_steps_.size());

    AdaptorType adaptor;

    std::for_each(time_steps_.begin(), time_steps_.end(),
                  [&](const auto& step){

        rewards.push_back(step.reward());

    });

    return adaptor(rewards);
}

template<typename StateType>
std::vector<TimeStepTp>
VectorTimeStep<StateType>::stack_time_step_types()const{

    std::vector<TimeStepTp> step_types;
    step_types.reserve(time_steps_.size());



    std::for_each(time_steps_.begin(), time_steps_.end(),
                  [&](const auto& step){

        step_types.push_back(step.type());

    });

    return step_types;
}

}

#endif // VECTOR_TIME_STEP_H
