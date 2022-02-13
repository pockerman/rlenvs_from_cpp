#ifndef GRID_WORLD_ENV_H
#define GRID_WORLD_ENV_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/env_mixin.h"
#include "gymfcpp/extern/enum.h"

///
/// Different namespace so that we differentiate
/// from OpenAI-Gym environment
///
namespace rlenvs
{

// still we may want to use some utilities
using namespace gymfcpp;

/**
  * Implements the Gridworld environment from the
  * book Deep Reinforcement Learning in Action by Manning publications.
  * You can find the original environment here:
  * https://github.com/DeepReinforcementLearning/DeepReinforcementLearningInAction
  * */

template<uint_t side_size>
struct GridworldData
{
    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<4> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_type
    ///
    typedef discrete_state_space_frozen_lake<side_size> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef typename state_space_type::item_type state_type;

    ///
    /// \brief state_boost_python_t
    ///
    typedef boost::python::list state_boost_python_type;

    ///
    /// \brief name
    ///
    static  const std::string name;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

    ///
    /// \brief state_transform_from_boost
    /// \param boost_type
    /// \return
    ///
    static state_type state_transform_from_boost(state_boost_python_type /*boost_type*/);

    ///
    /// \brief extract_state
    /// \param gym_namespace
    /// \return
    ///
    static state_type extract_state(obj_t /*gym_namespace*/, std::string /*result_name*/);

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

template<uint_t side_size>
class Gridworld final: protected EnvMixin<GridworldData<side_size>>
{
public:

    ///
    ///
    ///
    Gridworld(std::string version, bool create=true);


    ///
    /// \brief Expose the functionality this class is using
    /// from the Mixin
    ///

    using EnvMixin<GridworldData<side_size>>::full_name;
    using EnvMixin<GridworldData<side_size>>::is_created;
    using EnvMixin<GridworldData<side_size>>::version;
    using EnvMixin<GridworldData<side_size>>::idx;
    using EnvMixin<GridworldData<side_size>>::py_env_name;
    using EnvMixin<GridworldData<side_size>>::py_reset_result_name;
    using EnvMixin<GridworldData<side_size>>::py_step_result_name;
    using EnvMixin<GridworldData<side_size>>::py_state_name;

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    void make();

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const noexcept{ return side_size == 4 ? 16 : 64; }

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    time_step_type step(action_type action, bool query_extra=false);

    ///
    /// \brief render
    ///
    void render();

    void close();

    void reset()

};

}

#endif // GRID_WORLD_ENV_H
