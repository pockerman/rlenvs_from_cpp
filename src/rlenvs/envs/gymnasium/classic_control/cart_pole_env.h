#ifndef CART_POLE_ENV_H
#define CART_POLE_ENV_H

/**
  *
  * CartPole environment. The original environment
  *  is described here: https://github.com/openai/gym/blob/master/gym/envs/classic_control/cartpole.py
  *  The state variables are:
  *
  * Observation:
  *      Type: Box(4)
  *      Num     Observation               Min                     Max
  *      0       Cart Position             -2.4                    2.4
  *      1       Cart Velocity             -Inf                    Inf
  *      2       Pole Angle                -0.209 rad (-12 deg)    0.209 rad (12 deg)
  *      3       Pole Angular Velocity     -Inf                    Inf
  *  Actions:
  *      Type: Discrete(2)
  *      Num   Action
  *      0     Push cart to the left
  *      1     Push cart to the right
  *      Note: The amount the velocity that is reduced or increased is not
  *      fixed; it depends on the angle the pole is pointing. This is because
  *      the center of gravity of the pole increases the amount of energy needed
  *      to move the cart underneath it
  *  Reward:
  *      Reward is 1 for every step taken, including the termination step
  *  Starting State:
  *      All observations are assigned a uniform random value in [-0.05..0.05]
  *  Episode Termination:
  *      Pole Angle is more than 12 degrees.
  *      Cart Position is more than 2.4 (center of the cart reaches the edge of
  *      the display).
  *      Episode length is greater than 200.
  *      Solved Requirements:
  *      Considered solved when the average return is greater than or equal to
  *      195.0 over 100 consecutive trials.
  **/

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/continuous_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/envs/gymnasium/gymnasium_env_base.h"


#include <string>
#include <vector>
#include <tuple>
#include <any>

namespace rlenvs_cpp{

/// Forward declaration
template<typename StateTp> class TimeStep;

namespace envs{
namespace gymnasium{

BETTER_ENUM(CartPoleActionsEnum, int, LEFT=0, RIGHT=1, INVALID_ACTION=2);

///
/// \brief The CartPoleData struct. Wraps various data
/// related to the configuration of the CartPole environment.
///
struct CartPoleData
{

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<2> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_type (cart_position, cart_velocity, pole angle, pole angular velocity)
    ///
    typedef ContinuousSpace<4> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef state_space_type::item_t state_type;



    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;


    ///
    ///
    ///
    CartPoleData()=default;

};

///
/// \brief The CartPole class Interface for CartPole environment
///
class CartPole final: public GymnasiumEnvBase<CartPoleData::time_step_type>
{

public:

    ///
    /// \brief env_data_t
    ///
    typedef CartPoleData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef CartPoleData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef CartPoleData::action_type action_type;

    ///
    /// \brief state_space_t
    ///
    typedef CartPoleData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef CartPoleData::state_type state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef CartPoleData::time_step_type time_step_type;

    ///
    /// \brief CartPole. Constructor
    ///
    CartPole(const std::string& api_base_url );

    ///
    /// \brief ~CartPole. Destructor
    ///
    ~CartPole()=default;

    ///
    /// \brief make. Build the environment
    ///
    void make(const std::string& version,
              const std::unordered_map<std::string, std::any>& options) override final;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step
    ///
    time_step_type step(const CartPoleActionsEnum action);



protected:

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;

};



}
}
}

#endif // CART_POLE_H
