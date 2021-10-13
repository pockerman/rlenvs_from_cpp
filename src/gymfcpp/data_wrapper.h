#ifndef DATA_WRAPPER_H
#define DATA_WRAPPER_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"
#include <string>

namespace gymfcpp {
namespace detail{

///
/// \brief The data_wrapper struct. Helper struct to wrap common
/// functionality for the implemented environment
///
template<typename TimeStepType>
struct data_wrapper
{

    ///
    /// \brief v
    ///
    std::string v;

    ///
    /// \brief is_created_
    ///
    bool is_created;

    ///
    /// \brief gym_namespace_
    ///
    obj_t gym_namespace;

    ///
    /// \brief world_ The obj populated when make is called
    ///
    obj_t world;

    ///
    /// \brief current_state_ Keep tract what the current
    /// state of the world is
    ///
    TimeStepType current_state;

};
}

}

#endif // DATA_WRAPPER_H
