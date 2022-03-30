#ifndef CLIFF_WORLD_H
#define CLIFF_WORLD_H

/**
 *
 * This is a simple implementation of the Gridworld Cliff einforcement learning task.
 *
 *   ### Description
 *   The board is a 4x12 matrix, with (using NumPy matrix indexing):
 *
 *   - [3, 0] as the start at bottom-left
 *   - [3, 11] as the goal at bottom-right
 *   - [3, 1..10] as the cliff at bottom-center
 *
 *   If the agent steps on the cliff it returns to the start.
 *   An episode terminates when the agent reaches the goal.
 *
 * ### Actions
 *
 * There are 4 discrete deterministic actions:
 *   - 0: move up
 *   - 1: move right
 *   - 2: move down
 *   - 3: move left
 *
 *   ### Observations
 *
 *   There are 3x12 + 1 possible states. In fact, the agent cannot be at the cliff, nor at
 *   the goal (as this results the end of episode). They remain all the positions of the first 3 rows plus the bottom-left cell.
 *   The observation is simply the current position encoded as [flattened index](https://numpy.org/doc/stable/reference/generated/numpy.unravel_index.html).
 *
 *  ### Reward
 *
 *   Each time step incurs -1 reward, and stepping into the cliff incurs -100 reward.
 *
 */

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/env_mixin.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/time_step.h"


#include "boost/noncopyable.hpp"
#include <string>


namespace gymfcpp {

///
/// \brief The CliffWorldData struct
///
struct CliffWorldData
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
    typedef DiscreteSpace<48> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef typename state_space_type::item_t state_type;

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
    //static state_type state_transform_from_boost(state_boost_python_type /*boost_type*/);

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
    //static state_type extract_state_from_step(obj_t gym_namespace, std::string py_state_name, std::string result_name);

};


///
/// \brief The CliffWorld class
///
class CliffWorld: protected EnvMixin<CliffWorldData>
{

public:

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_type;

    ///
    /// \brief env_data_t
    ///
    typedef CliffWorldData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef CliffWorldData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef CliffWorldData::action_type action_type;

    ///
    /// \brief state_space_t
    ///
    typedef CliffWorldData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef CliffWorldData::state_type state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef CliffWorldData::time_step_type time_step_type;

    ///
    /// \brief FrozenLake
    /// \param version
    ///
    CliffWorld(std::string version, obj_t gym_namespace, bool do_create=true);

    ///
    /// \brief ~CliffWorld. Destructor
    ///
    ~CliffWorld();

    ///
    /// \brief Expose the functionality this class is using
    /// from the Mixin
    ///
    using EnvMixin<CliffWorldData>::close;
    using EnvMixin<CliffWorldData>::full_name;
    using EnvMixin<CliffWorldData>::reset;
    using EnvMixin<CliffWorldData>::is_created;
    using EnvMixin<CliffWorldData>::version;
    using EnvMixin<CliffWorldData>::gym_namespace;
    using EnvMixin<CliffWorldData>::render;
    using EnvMixin<CliffWorldData>::idx;
    using EnvMixin<CliffWorldData>::py_env_name;
    using EnvMixin<CliffWorldData>::py_reset_result_name;
    using EnvMixin<CliffWorldData>::py_step_result_name;
    using EnvMixin<CliffWorldData>::py_state_name;
    std::string py_dynamics_name;

    ///
    /// \brief make
    ///
    void make();

    ///
    /// \brief n_states
    ///
    uint_t n_states()const{return state_space_type::size;}

    ///
    /// \brief n_actions
    /// \return
    ///
    uint_t n_actions()const noexcept{return action_space_type::size; }

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    time_step_type step(action_type action, bool query_extra=false);

    ///
    /// \brief P
    /// \param sidx
    /// \param aidx
    ///
    dynamics_type p(uint_t sidx, uint_t aidx)const;

    ///
    /// \brief render. Render the environment
    ///
    //void render(std::string mode="human");

    ///
    /// \brief close. Close down the python environment
    ///
    //void close();

private:

};

}

#endif // CLIFF_WORLD_H
