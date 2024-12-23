#ifndef TOY_TEXT_BASE_ENV_H
#define TOY_TEXT_BASE_ENV_H

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/envs/gymnasium/gymnasium_env_base.h"
#include "rlenvs/discrete_space.h"

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <any>
#include <boost/noncopyable.hpp>

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium {
	
///
/// \brief ToyTextEnvBase class. Base class
/// for toy environments from Gymnasium. These environments
/// have a discrete action space

template<typename TimeStepType, typename StateSpaceType, uint_t space_dim>
class ToyTextEnvBase: public GymnasiumEnvBase<TimeStepType, StateSpaceType, DiscreteSpace<space_dim>>{
public:

    ///
	/// \brief The time step type we return every time a step in the
	/// environment is performed
	///
    typedef TimeStepType time_step_type;
	
	///
	/// \brief The type describing the state space for the environment
	///
	typedef StateSpaceType state_space_type;
	
	///
	/// \brief The type of the action space for the environment
	///
	typedef DiscreteSpace<space_dim> action_space_type;

    ///
	/// \brief The type of the action to be undertaken in the environment
	///
    typedef typename action_space_type::space_item_type action_type;
	

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    virtual ~ToyTextEnvBase()=default;

    ///
    /// \brief P
    /// \param sidx
    /// \param aidx
    ///
    dynamics_t p(uint_t sidx, uint_t aidx)const;
	
	///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    
protected:

    ///
    /// \brief Constructor
    ///
    ToyTextEnvBase(const uint_t cidx, const std::string& name, 
	               const std::string& api_url,
				   const std::string& resource_path);

    ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const=0;

    ///
    /// \brief build the time step from the server response
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response)const=0;

};

template<typename TimeStepType>
ToyTextEnvBase<TimeStepType>::ToyTextEnvBase(const uint_t cidx, const std::string& name, 
                                             const std::string& api_url, const std::string& resource_path)
:
GymnasiumEnvBase<TimeStepType>(cidx, name, api_url, resource_path)
{}


template<typename TimeStepType>
typename ToyTextEnvBase<TimeStepType>::dynamics_t
ToyTextEnvBase<TimeStepType>::p(uint_t sidx, uint_t aidx)const{

#ifdef RLENVSCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif

    const std::string url(this->get_url());
    const auto request_url = url + "/dynamics?stateId="+std::to_string(sidx)+"&actionId="+std::to_string(aidx);
    http::Request request{request_url};
    const auto response = request.send("GET");

    if(response.status.code != 200){
        throw std::runtime_error("Environment server failed to return  environment dynamics");
    }

    return build_dynamics_from_response_(response);
}


} // gymnasium
} // envs
} // rlenvs_cpp
#endif
