#ifndef ACROBOT_ENV_H
#define ACROBOT_ENV_H

/**
 *  Vector Acrobot environment. This class simply wraps copies of the 
 *  Acrobot class. 
 * See: https://github.com/pockerman/rlenvs_from_cpp/blob/master/src/rlenvs/envs/gymnasium/classic_control/acrobot_env.h
 * for more information
 **/

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/continuous_space.h"
#include "rlenvs/vector_time_step.h"
#include "rlenvs/envs/gymnasium/classic_control/acrobot_env_actions_enum.h"
#include "rlenvs/envs/gymnasium/gymnasium_vector_env_base.h"
#include "rlenvs/extern/HTTPRequest.hpp"


#include <string>
#include <vector>
#include <tuple>
#include <any>


namespace rlenvscpp{

/// Forward declaration
template<typename StateTp> class VectorTimeStep;

namespace envs{
namespace gymnasium{


typedef VectorTimeStep<ContinuousSpace<6>::item_t> acrobot_v_time_step_type;

///
/// \brief The CartPole class Interface for CartPole environment
///
class AcrobotV final: public GymnasiumVecEnvBase<VectorTimeStep<ContinuousSpace<6>::item_t>>
{

public:

	///
    /// \brief name
    ///
	static  const std::string name;
	
	///
    /// \brief action_space_t. The type of the action space
    ///
	typedef DiscreteSpace<3> action_space_type;
	
	///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    
	///
    /// \brief state_space_type (x =cos(theta), y =sin(theta), angular velocity)
    ///
    typedef ContinuousSpace<6> state_space_type;

    ///
    /// \brief state_type for a single environment
    ///
    typedef state_space_type::item_t state_type;
	
	 ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef acrobot_v_time_step_type time_step_type;

    ///
    /// \brief Acrobot. Constructor
    ///
    AcrobotV(const std::string& api_base_url);

    ///
    /// \brief make. Build the environment
    ///
    void make(const std::string& version,
              const std::unordered_map<std::string, std::any>&) override final;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step. Step in the environment following the given action
    ///
    time_step_type step(const std::vector<action_type>& action);
	
	///
    /// \brief step. Step in the environment following the given action
    ///
    time_step_type step(const std::vector<AcrobotActionsEnum>& action);


    ///
	/// \brief Synchronize the environment
	///
    void sync(const std::unordered_map<std::string, std::any>& =
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
