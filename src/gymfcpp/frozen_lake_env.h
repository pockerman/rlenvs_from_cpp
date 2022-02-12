#ifndef FROZEN_LAKE_H
#define FROZEN_LAKE_H

/**
  * Wrapper to the FrozenLake OpenAI-Gym environment.
  * The origina environment can be found at:
  * https://github.com/openai/gym/blob/master/gym/envs/toy_text/frozen_lake.py
  *  Frozen lake involves crossing a frozen lake from Start(S) to goal(G) without falling into any holes(H).
  * The agent may not always move in the intended direction due to the slippery nature of the frozen lake
  *
  *  The agent take a 1-element vector for actions.
  *  The action space is `(dir)`, where `dir` decides direction to move in which can be:
  *  - 0: LEFT
  *  - 1: DOWN
  *  - 2: RIGHT
  *  - 3: UP
  *
  * The observation is a value representing the agents current position as
  *      current_row * nrows + current_col
  *
  *  Reward schedule:
  *  - Reach goal(G): +1
  *  - Reach hole(H): 0
  *
  * ### Arguments
  *  ```
  *  gym.make('FrozenLake-v0', desc=None,map_name="4x4", is_slippery=True)
  *  ```
  *  `desc`: Used to specify custom map for frozen lake. For example,
  *      desc=["SFFF", "FHFH", "FFFH", "HFFG"].
  *  `map_name`: ID to use any of the preloaded maps.
  *      "4x4":[
  *          "SFFF",
  *          "FHFH",
  *          "FFFH",
  *          "HFFG"
  *          ]
  *      "8x8": [
  *          "SFFFFFFF",
  *          "FFFFFFFF",
  *          "FFFHFFFF",
  *          "FFFFFHFF",
  *          "FFFHFFFF",
  *          "FHHFFFHF",
  *          "FHFFHFHF",
  *          "FFFHFFFG",
  *      ]
  *  `is_slippery`: True/False. If True will move in intended direction with
  *  probability of 1/3 else will move in either perpendicular direction with
  *  equal probability of 1/3 in both directions.
  *      For example, if action is left and is_slippery is True, then:
  *      - P(move left)=1/3
  *      - P(move up)=1/3
  *      - P(move down)=1/3
  *
  * */

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/env_mixin.h"
#include "gymfcpp/discrete_space.h"
#include "gymfcpp/time_step.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace gymfcpp {

struct FrozenLakeData
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
    typedef DiscreteSpace<1> state_space_type;

    ///
    /// \brief state_type
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
/// \brief The FrozenLake class. Wrapper to OpenAI-Gym FrozenLake
/// environment
///
class FrozenLake: protected EnvMixin<FrozenLakeData>
{
public:

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief env_data_t
    ///
    typedef FrozenLakeData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef FrozenLakeData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef FrozenLakeData::action_type action_type;

    ///
    /// \brief state_space_t
    ///
    typedef FrozenLakeData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef FrozenLakeData::state_type state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef FrozenLakeData::time_step_type time_step_type;

    ///
    /// \brief FrozenLake. Constructor
    ///
    FrozenLake(const std::string& version, obj_t main_namespace, bool do_create=true,
               std::string map_type="4x4", bool is_slippery=true);

    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    ~FrozenLake();

    ///
    /// \brief Expose the functionality this class is using
    /// from the Mixin
    ///
    using EnvMixin<FrozenLakeData>::close;
    using EnvMixin<FrozenLakeData>::full_name;
    using EnvMixin<FrozenLakeData>::reset;
    using EnvMixin<FrozenLakeData>::is_created;
    using EnvMixin<FrozenLakeData>::version;
    using EnvMixin<FrozenLakeData>::gym_namespace;
    using EnvMixin<FrozenLakeData>::render;
    using EnvMixin<FrozenLakeData>::idx;
    using EnvMixin<FrozenLakeData>::py_env_name;
    using EnvMixin<FrozenLakeData>::py_reset_result_name;
    using EnvMixin<FrozenLakeData>::py_step_result_name;
    using EnvMixin<FrozenLakeData>::py_state_name;

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    void make();

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief reset
    /// \return
    ///
    time_step_type reset();

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
    dynamics_t p(uint_t sidx, uint_t aidx)const;

    ///
    /// \brief render. Render the environment
    ///
    void render(std::string mode="human");

    ///
    /// \brief map_type
    /// \return
    ///
    std::string map_type()const noexcept{return map_type_;}

    ///
    /// \brief is_slipery
    /// \return
    ///
    bool is_slippery()const noexcept{return is_slippery_;}


private:

    ///
    /// \brief map_type_
    ///
    const std::string map_type_;

    ///
    /// \brief is_slipery_
    ///
    const bool is_slippery_;


    ///
    /// \brief construct_python_string_
    /// \return
    ///
    std::string construct_python_string_()const noexcept;


};

}

#endif // FROZEN_LAKE_H
