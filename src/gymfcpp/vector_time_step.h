#ifndef VECTOR_TIME_STEP_H
#define VECTOR_TIME_STEP_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"

#include <vector>

namespace rlenvs{

using namespace gymfcpp;

template<typename StateType, typename StateAdaptor=void>
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

private:

    std::vector<TimeStep<StateType>> time_steps_;

};

template<typename StateType, typename StateAdaptor>
void
VectorTimeStep<StateType, StateAdaptor>::add_time_step(const TimeStep<StateType>& step){
    time_steps_.push_back(step);
}

}

#endif // VECTOR_TIME_STEP_H
