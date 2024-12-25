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

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/extern/enum.h"
#include "rlenvs/envs/gymnasium/toy_text/toy_text_base.h"

#include <string>
#include <tuple>
#include <any>
#include <unordered_map>

namespace rlenvscpp{
namespace envs {
namespace gymnasium {


BETTER_ENUM(CliffWorldActionsEnum, int, UP=0, RIGHT=1, DOWN=2, LEFT=3, INVALID_ACTION=4);

/**
  * @brief The CliffWorldData struct
  * Information  for the data comes from:
  * https://github.com/Farama-Foundation/Gymnasium/blob/main/gymnasium/envs/toy_text/cliffwalking.py
  *
  */
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
    typedef DiscreteSpace<37> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef typename state_space_type::item_t state_type;



    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

};


///
/// \brief The CliffWorld class
///
class CliffWorld final: public ToyTextEnvBase<CliffWorldData::time_step_type>
{

public:

    ///
    /// \brief name
    ///
    static  const std::string name;

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


    /**
     * @brief Convert the action index to a valid FrozenLakeActionsEnum
     *
     * */
    static CliffWorldActionsEnum action_from_int(uint_t aidx);

    ///
    /// \brief CliffWorld
    ///
    CliffWorld(const std::string& api_base_url);

    ///
    /// \brief ~CliffWorld. Destructor
    ///
    ~CliffWorld()=default;

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) override final;

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
    time_step_type step(CliffWorldActionsEnum action);


    /**
     * @brief Step in the environment following the given action
     *
     * */
    time_step_type step(uint_t action);


protected:
	
	///
	/// \brief Maximum episodes per step
	///
	uint_t max_episode_steps_;

    ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const override final;

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;
};

}
}
}
#endif // CLIFF_WORLD_H
