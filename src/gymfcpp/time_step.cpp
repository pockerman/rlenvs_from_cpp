#include "gymfcpp/time_step.h"

namespace gymfcpp
{

TimeStep::TimeStep()
    :
      type_(TimeStepTp::INVALID_TYPE),
      reward_(0.0),
      obs_(static_cast<uint_t>(-1)),
      extra_()
{}

TimeStep::TimeStep(TimeStepTp type, real_t reward, uint_t obs)
    :
      type_(type),
      reward_(reward),
      obs_(obs),
      extra_()
{}

TimeStep::TimeStep(TimeStepTp type, real_t reward, uint_t obs, std::unordered_map<std::string, std::any>&& extra)
    :
    type_(type),
    reward_(reward),
    obs_(obs),
    extra_(extra)
{}

}
