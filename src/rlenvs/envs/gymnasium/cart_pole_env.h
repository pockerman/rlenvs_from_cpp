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
#include "rlenvs/rlenvs_types.h"
#include "rlenvs/envs/env_mixin.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/continuous_space.h"
#include "rlenvs/time_step.h"

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>
#include <array>
#include <tuple>

namespace rlenvs_cpp{

/// Forward declaration
template<typename StateTp> class TimeStep;

namespace envs{
namespace gymnasium{

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
    
    ///
    ///
    ///
    CartPoleData()=default;

};

///
/// \brief The CartPole class Interface for CartPole environment
///
class CartPole: protected EnvMixin<CartPoleData>
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
    /// \brief The class Screen. Wrapper to the screen captured when calling render
    ///
    class Screen;

    ///
    /// \brief CartPole. Constructor
    ///
    CartPole(const std::string& version, obj_t main_namespace, bool do_create=true);

    ///
    /// \brief CartPole
    /// \param id
    /// \param version
    /// \param gym_namespace
    /// \param do_create
    ///
    CartPole(uint_t id, const std::string& version, obj_t main_namespace, bool do_create=true);

    ///
    /// \brief ~CartPole. Destructor
    ///
    ~CartPole();

    ///
    /// \brief Expose the functionality this class is using
    /// from the Mixin
    ///
    using EnvMixin<CartPoleData>::close;
    using EnvMixin<CartPoleData>::full_name;
    using EnvMixin<CartPoleData>::reset;
    using EnvMixin<CartPoleData>::is_created;
    using EnvMixin<CartPoleData>::version;
    using EnvMixin<CartPoleData>::gym_namespace;
    using EnvMixin<CartPoleData>::render;
    using EnvMixin<CartPoleData>::idx;
    using EnvMixin<CartPoleData>::py_env_name;
    using EnvMixin<CartPoleData>::py_reset_result_name;
    using EnvMixin<CartPoleData>::py_step_result_name;
    using EnvMixin<CartPoleData>::py_state_name;

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
    time_step_type step(const action_type action);

    ///
    /// \brief the screen
    ///
    Screen get_screen()const;

private:

    using EnvMixin<CartPoleData>::current_state;
    using EnvMixin<CartPoleData>::env;

};


class CartPole::Screen
{
public:

    typedef std::vector<std::vector<std::vector<real_t>>> screen_vector_t;

    ///
    ///
    ///
    Screen()=default;

    ///
    /// \brief Screen
    ///
    Screen(obj_t screen, std::array<uint_t, 3>&& shp);

    ///
    /// \brief shape. The shape of screen.
    ///
    std::array<uint_t, 3> shape()const noexcept{return shape_;};

    ///
    /// \brief get_as_vector. Returns the screen as floats
    ///
    const std::vector<std::vector<std::vector<real_t>>>& get_as_vector()const;
    
#ifdef USE_PYTORCH
    torch_tensor_t get_as_torch_tensor()const;
#endif
    
    ///
    /// \brief is_valid. Returns true if the computed screen is valid
    ///
    bool is_valid()const noexcept{return is_valid_screen_;}
    
    ///
    /// \brief invalidate. Invalidate the screen.
    ///
    void invalidate() noexcept;

private:

    obj_t screen_;
    std::array<uint_t, 3> shape_;
    bool is_valid_screen_{false};
    mutable std::vector<std::vector<std::vector<real_t>>> screen_vec_;

};

}
}
}

#endif // CART_POLE_H
