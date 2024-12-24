#ifndef PENDULUM_ENV_H
#define PENDULUM_ENV_H

/**
  *
  * Pendulum environment. The original environment
  *  is described here: https://github.com/openai/gym/blob/master/gym/envs/classic_control/pendulum.py
  *  The state variables are:
  *
  *      ### Description

    The inverted pendulum swingup problem is based on the classic problem in control theory.
    The system consists of a pendulum attached at one end to a fixed point, and the other end being free.
    The pendulum starts in a random position and the goal is to apply torque on the free end to swing it
    into an upright position, with its center of gravity right above the fixed point.

    The diagram below specifies the coordinate system used for the implementation of the pendulum's
    dynamic equations.

    ![Pendulum Coordinate System](./diagrams/pendulum.png)

    -  `x-y`: cartesian coordinates of the pendulum's end in meters.
    - `theta` : angle in radians.
    - `tau`: torque in `N m`. Defined as positive _counter-clockwise_.

    ### Action Space

    The action is a `ndarray` with shape `(1,)` representing the torque applied to free end of the pendulum.

    | Num | Action | Min  | Max |
    |-----|--------|------|-----|
    | 0   | Torque | -2.0 | 2.0 |


    ### Observation Space

    The observation is a `ndarray` with shape `(3,)` representing the x-y coordinates of the pendulum's free
    end and its angular velocity.

    | Num | Observation      | Min  | Max |
    |-----|------------------|------|-----|
    | 0   | x = cos(theta)   | -1.0 | 1.0 |
    | 1   | y = sin(theta)   | -1.0 | 1.0 |
    | 2   | Angular Velocity | -8.0 | 8.0 |

    ### Rewards

    The reward function is defined as:

    *r = -(theta<sup>2</sup> + 0.1 * theta_dt<sup>2</sup> + 0.001 * torque<sup>2</sup>)*

    where `$\theta$` is the pendulum's angle normalized between *[-pi, pi]* (with 0 being in the upright position).
    Based on the above equation, the minimum reward that can be obtained is
    *-(pi<sup>2</sup> + 0.1 * 8<sup>2</sup> + 0.001 * 2<sup>2</sup>) = -16.2736044*,
    while the maximum reward is zero (pendulum is upright with zero velocity and no torque applied).

    ### Starting State

    The starting state is a random angle in *[-pi, pi]* and a random angular velocity in *[-1,1]*.

    ### Episode Truncation

    The episode truncates at 200 time steps.

    ### Arguments

    - `g`: acceleration of gravity measured in *(m s<sup>-2</sup>)* used to calculate the pendulum dynamics.
      The default value is g = 10.0 .

    ```
    gym.make('Pendulum-v1', g=9.81)
    ```

    ### Version History

    * v1: Simplify the math equations, no difference in behavior.
    * v0: Initial versions release (1.0.0).
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

namespace rlenvscpp{

/// Forward declaration
template<typename StateTp> class TimeStep;

namespace envs{
namespace gymnasium{

	
//typedef ContinuousSpace<3> state_space_type;
//typedef state_space_type::item_t state_type;
typedef TimeStep<ContinuousSpace<3>::item_t> pendulum_time_step_type;

///
/// \brief The CartPole class Interface for CartPole environment
///
class Pendulum final: public GymnasiumEnvBase<pendulum_time_step_type>
{

public:

	///
    /// \brief name
    ///
	static  const std::string name;

	///
    /// \brief action_space_t. The type of the action space
    ///
	typedef ContinuousSpace<1> action_space_type;
	
	///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    
	///
    /// \brief state_space_type (x =cos(theta), y =sin(theta), angular velocity)
    ///
    typedef ContinuousSpace<3> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef state_space_type::item_t state_type;
	
	 ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

    ///
    /// \brief CartPole. Constructor
    ///
    Pendulum(const std::string& api_base_url );

    ///
    /// \brief ~CartPole. Destructor
    ///
    ~Pendulum()=default;

    ///
    /// \brief make. Build the environment
    ///
    void make(const std::string& version,
              const std::unordered_map<std::string, std::any>& /*options*/ = 
			  std::unordered_map<std::string, std::any>()) override final;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step. Step in the environment following the given action
    ///
    time_step_type step(const action_type action);


    /**
     * @brief Synchronize the environment
     *
     */
    void sync(const std::unordered_map<std::string, std::any>& /*options*/=
	          std::unordered_map<std::string, std::any>()){}



protected:

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;

};



}
}
}

#endif // PENDULUM_ENV_H
