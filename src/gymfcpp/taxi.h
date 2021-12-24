#ifndef TAXI_H
#define TAXI_H

#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/env_mixin.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/continuous_space.h"
#include "gymfcpp/time_step.h"

namespace gymfcpp
{

///
/// \brief The TaxiData struct. Wrapper for the environment data
///
struct TaxiData
{
    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<6> action_space_t;

    ///
    /// \brief action_t
    ///
    typedef action_space_t::item_t action_t;

    ///
    /// \brief state_space_t
    ///
    typedef ContinuousSpace<4> state_space_t;

    ///
    /// \brief state_t
    ///
    typedef uint_t state_t;

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
    /// \brief extract_state
    /// \param gym_namespace
    /// \return
    ///
    static state_t extract_state(obj_t gym_namespace, std::string result_name);

};


///
/// \brief The Taxi class
///
class Taxi: protected EnvMixin<TaxiData>
{
public:

    ///
    /// \brief env_data_t
    ///
    typedef TaxiData  env_data_t;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef TaxiData::action_space_t action_space_t;

    ///
    /// \brief action_t
    ///
    typedef TaxiData::action_t action_t;

    ///
    /// \brief state_space_t
    ///
    typedef TaxiData::state_space_t state_space_t;

    ///
    /// \brief state_t
    ///
    typedef TaxiData::state_t state_t;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TaxiData::time_step_t time_step_t;

    ///
    /// \brief Taxi
    /// \param version
    /// \param main_namespace
    /// \param do_create
    ///
    Taxi(std::string version, obj_t main_namespace, bool do_create=true);

    ///
    /// \brief Expose the functionality this class is using
    /// from the Mixin
    ///
    using EnvMixin<TaxiData>::close;
    using EnvMixin<TaxiData>::full_name;
    using EnvMixin<TaxiData>::reset;
    using EnvMixin<TaxiData>::is_created;
    using EnvMixin<TaxiData>::version;
    using EnvMixin<TaxiData>::gym_namespace;
    using EnvMixin<TaxiData>::render;

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

    ///
    /// \brief sample
    /// \return
    ///
    action_t sample()const noexcept{return action_space_t::sample();}

};


}
#endif // TAXI_H
