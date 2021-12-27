#ifndef TAXI_H
#define TAXI_H

#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/env_mixin.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/continuous_space.h"
#include "gymfcpp/time_step.h"

#include <map>
#include <string>

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
    typedef DiscreteSpace<6> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_t
    ///
    typedef ContinuousSpace<4> state_space_type;

    ///
    /// \brief state_t
    ///
    typedef uint_t state_type;

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
    //static std::string py_env_name;

    ///
    /// \brief py_step_result_name. The name of the result when stepping in the Python REPL
    ///
    //static std::string py_step_result_name;

    ///
    /// \brief py_step_reset_result_name
    ///
    //static std::string py_reset_result_name;

    ///
    /// \brief py_step_reset_result_name
    ///
    //static std::string py_state_name;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

    ///
    /// \brief extract_state
    /// \param gym_namespace
    /// \return
    ///
    //static state_type extract_state(obj_t gym_namespace, std::string result_name);

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
/// \brief The Taxi class
///
class Taxi: protected EnvMixin<TaxiData>
{
public:

    ///
    /// \brief env_data_t
    ///
    typedef TaxiData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef TaxiData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef TaxiData::action_type action_type;

    ///
    /// \brief state_space_t
    ///
    typedef TaxiData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef TaxiData::state_type state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TaxiData::time_step_type time_step_type;

    ///
    /// \brief Taxi
    /// \param version
    /// \param main_namespace
    /// \param do_create
    ///
    Taxi(std::string version, obj_t main_namespace, bool do_create=true);

    ///
    /// \brief Taxi Constructor
    /// \param main_namespace
    /// \param names
    ///
    Taxi(std::string version, obj_t main_namespace, std::map<std::string, std::string>&& names);

    ///
    /// \brief Taxi
    /// \param other
    ///
    Taxi(const Taxi& other);

    ///
    /// \brief Taxi
    /// \param other
    ///
    Taxi(Taxi&& other);

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
    using EnvMixin<TaxiData>::py_env_name;
    using EnvMixin<TaxiData>::py_reset_result_name;
    using EnvMixin<TaxiData>::py_step_result_name;
    using EnvMixin<TaxiData>::py_state_name;

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
    action_type sample_action()const noexcept{return action_space_type::sample();}

    ///
    /// \brief copy. Deep copy of the environment
    /// \return
    ///
    Taxi copy(std::map<std::string, std::string>&& names)const;

private:


    using EnvMixin<TaxiData>::current_state;

};


}
#endif // TAXI_H
