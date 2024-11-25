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

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/extern/enum.h"
#include "rlenvs/envs/gymnasium/toy_text/toy_text_base.h"

#include <string>
#include <vector>
#include <tuple>
#include <any>
#include <unordered_map>

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium {

template<uint_t side_size>
struct discrete_state_space_frozen_lake;

BETTER_ENUM(FrozenLakeActionsEnum, int,
            LEFT=0, DOWN=1, RIGHT=2, UP=3, INVALID_ACTION=4);


template<>
struct discrete_state_space_frozen_lake<4>
{

    ///
    /// \brief item_t
    ///
    typedef uint_t item_type;


    ///
    /// \brief size
    ///
    static constexpr uint_t size = 16;

    ///
    /// \brief sample
    /// \return
    ///
    static item_type sample(){return DiscreteSpace<16>::sample();}

    ///
    /// \brief
    ///
    static item_type sample(uint_t seed){return DiscreteSpace<16>::sample(seed);}

};

template<>
struct discrete_state_space_frozen_lake<8>
{
    ///
    /// \brief item_t
    ///
    typedef uint_t item_type;


    ///
    /// \brief size
    ///
    static constexpr uint_t size = 64;

    ///
    /// \brief sample
    /// \return
    ///
    static item_type sample(){return DiscreteSpace<64>::sample();}

    ///
    /// \brief
    ///
    static item_type sample(uint_t seed){return DiscreteSpace<64>::sample(seed);}

};

template<uint_t side_size>
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
    typedef discrete_state_space_frozen_lake<side_size> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef typename state_space_type::item_type state_type;

    ///
    /// \brief name
    ///
    //static  const std::string name;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;


};

///
/// \brief The FrozenLake class. Wrapper to OpenAI-Gym FrozenLake
/// environment
///
template<uint_t side_size>
class FrozenLake final: public ToyTextEnvBase<typename FrozenLakeData<side_size>::time_step_type>
{
public:
	
	///
    /// \brief name
    ///
    static  const std::string name;

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief env_data_t
    ///
    typedef FrozenLakeData<side_size>  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef typename FrozenLakeData<side_size>::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef typename FrozenLakeData<side_size>::action_type action_type;

    ///
    /// \brief state_space_t
    ///
    typedef typename FrozenLakeData<side_size>::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef typename FrozenLakeData<side_size>::state_type state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef typename FrozenLakeData<side_size>::time_step_type time_step_type;

    /**
     * @brief Convert the action index to a valid FrozenLakeActionsEnum
     *
     * */
    static FrozenLakeActionsEnum action_from_int(uint_t aidx);

    ///
    /// \brief Constructor.
    ///
    FrozenLake(const std::string& api_base_url);

    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    ~FrozenLake()=default;

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) override final;


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
    time_step_type step(FrozenLakeActionsEnum action);


    /**
     * @brief Step in the environment following the given action
     *
     * */
    time_step_type step(uint_t action);


    ///
    /// \brief map_type
    /// \return
    ///
    std::string map_type()const noexcept{return side_size == 4 ? "4x4" : "8x8";}

    ///
    /// \brief is_slipery
    /// \return
    ///
    bool is_slippery()const noexcept{return is_slippery_;}


protected:

    ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const override final;

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;

private:

    ///
    /// \brief is_slipery_
    ///
    bool is_slippery_;

};

}
}
}
#endif // FROZEN_LAKE_H
