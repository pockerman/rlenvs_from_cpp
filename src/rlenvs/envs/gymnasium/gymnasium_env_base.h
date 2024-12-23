// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#ifndef GYMNASIUM_ENV_BASE_H
#define GYMNASIUM_ENV_BASE_H


#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"
#include "rlenvs/envs/with_rest_api_mixin.h"
#include "rlenvs/envs/env_base.h"


#include <boost/noncopyable.hpp>
#include <string>
#include <string_view>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <any>


#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium {

template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
class GymnasiumEnvBase: public EnvBase<TimeStepType, StateSpaceType, ActionSpaceType>, 
                        public with_rest_api_mixin<TimeStepType>{
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
	typedef ActionSpaceType action_space_type;

    ///
	/// \brief The type of the action to be undertaken in the environment
	///
    typedef typename action_space_type action_type;

    ///
    /// \brief ~GymnasiumEnvBase. Destructor.
    ///
    virtual ~GymnasiumEnvBase();
	
	///
	/// \brief
	///
	virtual bool is_alive()const override;

    ///
    /// \brief close the environment
    ///
    virtual void close() override;


protected:

    ///
    /// \brief Constructor
    ///
    GymnasiumEnvBase(const uint_t cidx, 
	                 const std::string& name, 
					 const std::string& api_url,
					 const std::string& resource_path);

};

template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
GymnasiumEnvBase<TimeStepType, 
                 StateSpaceType, 
				 ActionSpaceType>::GymnasiumEnvBase(const uint_t cidx, const std::string& name, 
												    const std::string& api_url, const std::string& resource_path)
:
EnvBase<TimeStepType, StateSpaceType, ActionSpaceType>(cidx, name),
with_rest_api_mixin<TimeStepType>(api_url, resource_path)
{}

template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
GymnasiumEnvBase<TimeStepType, StateSpaceType, ActionSpaceType>::~GymnasiumEnvBase(){
    close();
}

template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
bool
GymnasiumEnvBase<TimeStepType, StateSpaceType, ActionSpaceType>::is_alive()const{
	
	
	auto url_ = this->get_url();
	auto copy_idx_str = std::to_string(this -> cidx());
	
    http::Request request{url_ + "/is-alive?cidx="+copy_idx_str};
    const auto response = request.send("GET");
    const auto str_response = std::string(response.body.begin(), response.body.end());
	
	 using json = nlohmann::json;

	// parse the response
    json j = json::parse(str_response);
	return j["result"];

}

template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
void
GymnasiumEnvBase<TimeStepType, StateSpaceType, ActionSpaceType>::close(){

     if(!is_created_){
        return;
    }

	auto copy_idx = this -> cidx();
	using json = nlohmann::json;
    json j;
	j["cidx"] = copy_idx;
	
    http::Request request{url_ + "/close"};
    const auto response = request.send("POST");
    is_created_ = false;

}


template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
typename GymnasiumEnvBase<TimeStepType, StateSpaceType, ActionSpaceType>::time_step_type
GymnasiumEnvBase<TimeStepType, StateSpaceType, ActionSpaceType>::reset(uint_t seed,
                                      const std::unordered_map<std::string, std::any>& /*options*/){

    if(!is_created_){
#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif
     return time_step_type();
    }
	
	auto copy_idx = this -> cidx();
	
    const auto request_url = url_ + "/reset";
    http::Request request{request_url};


    using json = nlohmann::json;
    json j;
    j["seed"] = seed;
	j["cidx"] = copy_idx;
	
    auto body = j.dump();
    const auto response = request.send("POST", body);

     if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to reset environment");
    }

    current_state_ = this->create_time_step_from_response_(response);
    return current_state_;

}



} // gymnasium
} // envs
} // rlenvs_cpp

#endif // GYMNASIUMENVBASE_H
