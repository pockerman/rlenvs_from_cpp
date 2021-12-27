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



///
/// \brief The MountainCarData struct. Wrapper for the environment data
///
struct MountainCarData
{
    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<3> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_t
    ///
    typedef ContinuousSpace<3> state_space_type;

    ///
    /// \brief state_t
    ///
    typedef state_space_type::item_t state_type;

    ///
    /// \brief state_boost_python_t
    ///
    typedef boost::python::list state_boost_python_type;

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
    typedef TimeStep<state_type> time_step_type;

    ///
    /// \brief state_transform_from_boost
    /// \param boost_type
    /// \return
    ///
    static state_type state_transform_from_boost(state_boost_python_type boost_type);

    ///
    /// \brief extract_state
    /// \param gym_namespace
    /// \return
    ///
    static state_type extract_state(obj_t gym_namespace, std::string result_name);

    ///
    /// \brief extract_state_from_reset
    /// \param gym_namespace
    /// \param py_env_n
    /// \return
    ///
    static state_type extract_state_from_reset(obj_t gym_namespace, std::string py_state_name, std::string result_name);

    ///
    /// \brief extract_state_from_step
    /// \param gym_namespace
    /// \param py_state_name
    /// \param result_name
    /// \return
    ///
    static state_type extract_state_from_step(obj_t gym_namespace, std::string py_state_name, std::string result_name);

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
    typedef MountainCarData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef MountainCarData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_t
    ///
    typedef MountainCarData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef state_space_type::item_t state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

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
    using EnvMixin<MountainCarData>::render;

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
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step
    ///
    time_step_type step(action_type action);

    ///
    /// \brief sample
    /// \return
    ///
    action_type sample()const noexcept{return MountainCarData::action_space_type::sample();}

private:

    ///
    /// \brief  The boost::python object environment
    ///
    using EnvMixin<MountainCarData>::env;

};
}
#endif // MOUNTAIN_CAR_H
