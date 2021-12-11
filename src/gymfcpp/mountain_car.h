#ifndef MOUNTAIN_CAR_H
#define MOUNTAIN_CAR_H

#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/env_mixin.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/continuous_space.h"
#include "gymfcpp/time_step.h"

#include <boost/noncopyable.hpp>
#include <string>

namespace gymfcpp
{

/// Forward declarations
//template<typename StateTp> class TimeStep;


struct MountainCarData
{
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
    /// \brief state_boost_python_t
    ///
    typedef boost::python::list state_boost_python_t;

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
    /// \brief py_step_reset_result_name
    ///
    static std::string py_state_name;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief state_transform_from_boost
    /// \param boost_type
    /// \return
    ///
    static state_t state_transform_from_boost(state_boost_python_t boost_type);

    ///
    /// \brief extract_state
    /// \param gym_namespace
    /// \return
    ///
    static state_t extract_state(obj_t gym_namespace, std::string result_name);

};

///
/// \brief The MountainCar class
///
class MountainCar: protected EnvMixin<MountainCarData>
{

public:

    ///
    /// \brief env_data_t
    ///
    typedef MountainCarData  env_data_t;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef MountainCarData::action_space_t action_space_t;

    ///
    /// \brief action_t
    ///
    typedef action_space_t::item_t action_t;

    ///
    /// \brief state_space_t
    ///
    typedef MountainCarData::state_space_t state_space_t;

    ///
    /// \brief state_t
    ///
    typedef state_space_t::item_t state_t;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief Expose the functionality this class is using
    /// from the Mixin
    ///
    using EnvMixin<MountainCarData>::close;
    using EnvMixin<MountainCarData>::full_name;
    using EnvMixin<MountainCarData>::reset;
    using EnvMixin<MountainCarData>::is_created;
    using EnvMixin<MountainCarData>::version;
    using EnvMixin<MountainCarData>::gym_namespace;

    ///
    /// \brief MountainCar. Constructor. Creates an environment.
    /// \param version The version of the environment
    /// \param gym_namespace The boost::python open-ai gym namespace
    /// \param do_create If true it calls make
    ///
    MountainCar(std::string version, obj_t gym_namespace, bool do_create=true);

    ///
    /// \brief ~MountainCar. Destructor.
    ///
    ~MountainCar();

    ///
    /// \brief make. Build the environment
    ///
    void make();

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_t::size;}

    ///
    /// \brief step
    ///
    time_step_t step(action_t action);

private:

    ///
    /// \brief  The boost::python object environment
    ///
    using EnvMixin<MountainCarData>::env;

};
}
#endif // MOUNTAIN_CAR_H
