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


    void reserve(uint_t ncopies){time_steps_.reserve(ncopies);}
    void add_time_step(TimeStep<StateType>&& step);

private:

    std::vector<TimeStep<StateType>> time_steps_;

};

template<typename StateType, typename StateAdaptor>
void
VectorTimeStep<StateType, StateAdaptor>::add_time_step(TimeStep<StateType>&& step){
    time_steps_.push_back(step);
}

}

#endif // VECTOR_TIME_STEP_H
