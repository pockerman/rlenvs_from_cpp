#ifndef MOUNTAIN_CAR_H
#define MOUNTAIN_CAR_H

#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/env_base.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/continuous_space.h"

#include <boost/noncopyable.hpp>
#include <string>

namespace gymfcpp
{

/// Forward declarations
template<typename StateTp> class TimeStep;

///
/// \brief The MountainCar class
///
class MountainCar: public EnvBase
{

public:

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<3> action_space_t;

    ///
    /// \brief action_t
    ///
    typedef action_space_t::item_t action_t;

    ///
    /// \brief state_space_t
    ///
    typedef ContinuousSpace<3> state_space_t;

    ///
    /// \brief state_t
    ///
    typedef state_space_t::item_t state_t;

    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief py_env_name. The name of the environment in Python REPL
    ///
    static std::string py_env_name;

    ///
    /// \brief py_step_result_name. The name of the result when stepping in the Python REPL
    ///
    static std::string py_step_result_name;

    ///
    /// \brief py_step_reset_result_name
    ///
    static std::string py_reset_result_name;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief MountainCar. Constructor. Creates an environment.
    /// \param version The version of the environment
    /// \param gym_namespace The boost::python open-ai gym namespace
    /// \param do_create If true it calls make
    ///
    MountainCar(std::string version, obj_t gym_namespace, bool do_create=true);

    ///
    /// \brief make. Build the environment
    ///
    void make();

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const{return action_space_t::size;}

    ///
    /// \brief reset
    /// \return
    ///
    time_step_t reset();

    ///
    /// \brief step
    ///
    time_step_t step(action_t action);



};
}
#endif // MOUNTAIN_CAR_H
