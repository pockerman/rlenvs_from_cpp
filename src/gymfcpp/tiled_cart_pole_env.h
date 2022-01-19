#ifndef TILED_CART_POLE_ENV_H
#define TILED_CART_POLE_ENV_H

/**
  *
  * TiledCartPole environment. Wrapper to CartPole environment using state aggregation. The original environment
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

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/cart_pole_env.h"
#include "gymfcpp/tiled_env_base.h"

#include <string>
#include <utility>
#include <vector>

namespace gymfcpp{

/// Forward declaration
template<typename StateTp> class TimeStep;

///
/// \brief The TiledCartPoleBoundaries struct. Boundaries for
/// the state variables in the CartPole environment
///
struct TiledCartPoleBoundaries
{

    std::pair<real_t,real_t> cart_pole_pos{-2.4, 2.4};
    std::pair<real_t,real_t> cart_pole_vel{-4.0, 4.0};
    std::pair<real_t,real_t> cart_pole_theta{-0.20943951, 0.20943951};
    std::pair<real_t,real_t> cart_pole_theta_vel{-4.0, 4.0};
};

///
/// \brief The TiledCartPole class
///
class TiledCartPole final: public TiledEnviromentBase<CartPole>
{
public:

    typedef TiledEnviromentBase<CartPole>::time_step_type time_step_type;
    typedef TiledEnviromentBase<CartPole>::state_type state_type;
    typedef TiledEnviromentBase<CartPole>::action_type action_type;

    ///
    /// \brief Constructor
    ///
    TiledCartPole(const std::string version, obj_t main_namespace, uint_t n_states,
                  uint_t state_idx=4, const TiledCartPoleBoundaries& boundaries=TiledCartPoleBoundaries());

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
    virtual time_step_type step(const action_type& action) override final;

    ///
    /// \brief create_bins. Create the bins for the variables
    ///
    virtual void create_bins() override final;

    ///
    /// \brief get_state Transforms the original state to the aggregate state
    /// \param obs. The original continuous state
    /// \return
    ///
    virtual state_type get_state(const CartPole::state_type& obs)const override final;

    ///
    /// \brief n_actions
    /// \return
    ///
    uint_t n_actions()const noexcept{return this->env_.n_actions();}

private:

    ///
    /// \brief state_idx_
    ///
    uint_t state_idx_;

    ///
    /// \brief boundaries_
    ///
    const TiledCartPoleBoundaries boundaries_;

    ///
    /// \brief bins for the pole position space
    ///
    std::vector<real_t> pole_theta_space_;

    ///
    /// \brief bins for the pole velocity space
    ///
    std::vector<real_t> pole_theta_velocity_space_;

    ///
    /// \brief bins for the cart position space
    ///
    std::vector<real_t> cart_pos_space_;

    ///
    /// \brief bins for the cart velocity space
    ///
    std::vector<real_t> cart_vel_space_;

    ///
    /// \brief current_state_
    ///
    time_step_type current_state_;

    void _build_pole_theta_space();
    void _build_cart_position_space();
    void _build_cart_velocity_space();
    void _build_pole_theta_velocity_space();
};



}

#endif // TILED_CART_POLE_ENV_H
