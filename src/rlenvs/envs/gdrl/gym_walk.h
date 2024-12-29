// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#ifndef GYM_WALK_H
#define GYM_WALK_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/time_step.h"
#include "rlenvs/envs/env_types.h"
#include "rlenvs/envs/env_base.h"
#include "rlenvs/envs/with_rest_api_mixin.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#include <boost/noncopyable.hpp>
#include <vector>
#include <tuple>
#include <string>
#include <any>
#include <unordered_map>
#include <memory>

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

namespace rlenvscpp{
namespace envs{
namespace gdrl{


///
/// \brief class GymWalk. Interface for the GymWalk environment
///
template<uint_t state_size>
class GymWalk final: public EnvBase<TimeStep<uint_t>, 
                                    ScalarDiscreteEnv<state_size, 2, 0, 0>
									>,
					 public with_rest_api_mixin<TimeStep<uint_t>>
{
public:
	
	///
    /// \brief name
    ///
    static  const std::string name;
	
	typedef EnvBase<TimeStep<uint_t>, 
                                    ScalarDiscreteEnv<state_size, 2, 0, 0>
									> base_type;
									
	   ///
	/// \brief The time step type we return every time a step in the
	/// environment is performed
	///
    typedef typename base_type::time_step_type time_step_type;
	
	///
	/// \brief The type describing the state space for the environment
	///
	typedef typename base_type::state_space_type state_space_type;
	
	///
	/// \brief The type of the action space for the environment
	///
	typedef typename base_type::action_space_type action_space_type;

    ///
	/// \brief The type of the action to be undertaken in the environment
	///
    typedef typename base_type::action_type action_type;
	
	///
	/// \brief The type of the state
	///
	typedef typename base_type::state_type state_type; 
									
	///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;								
									
    ///
	/// \brief Constructor
	///
    GymWalk(const std::string& api_base_url);
	
	///
	/// \brief Constructor
	///
	GymWalk(const std::string& api_base_url, 
			const uint_t cidx);
			
	///
	/// \brief copy ctor
	///
	GymWalk(const GymWalk& other);

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) override final;
					  
					  
	///
	/// \brief
	///
	virtual bool is_alive()const override final;
	
	///
    /// \brief close the environment
    ///
    virtual void close() override final;
	
	///
    /// \brief step
    /// \param action
    /// \return
    ///
    virtual time_step_type step(const action_type& action) override final;
	
	/// 
	/// \brief Reset the environment
	///
    virtual time_step_type reset(uint_t seed,
                                 const std::unordered_map<std::string, std::any>& options)override final;
								 
								 
	///
	/// \brief Create a new copy of the environment with the given
	/// copy index
	///
	virtual std::unique_ptr<base_type> make_copy(uint_t cidx)const override final;


    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const noexcept{ return state_space_type::size; }

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}


private:
	
    ///
    /// \brief build the dynamics from response
    ///
    dynamics_t build_dynamics_from_response_(const http::Response& response)const;

    ///
    /// \brief Handle the reset response from the environment server
    ///
    time_step_type create_time_step_from_response_(const http::Response& response) const;
};

template<uint_t state_size>
const std::string GymWalk<state_size>::name = "GymWalk";


template<uint_t state_size>
GymWalk<state_size>::GymWalk(const std::string& api_base_url)
:
EnvBase<TimeStep<uint_t>, 
		ScalarDiscreteEnv<state_size, 2, 0, 0>
	   >(0, "GymWalk"),
with_rest_api_mixin<TimeStep<uint_t>>(api_base_url, "/gdrl/gym-walk-env")
{}

template<uint_t state_size>
GymWalk<state_size>::GymWalk(const std::string& api_base_url, 
			const uint_t cidx)
:
EnvBase<TimeStep<uint_t>, 
		ScalarDiscreteEnv<state_size, 2, 0, 0>
	   >(cidx, "GymWalk"),
with_rest_api_mixin<TimeStep<uint_t>>(api_base_url, "/gdrl/gym-walk-env")
{}


template<uint_t state_size>
GymWalk<state_size>::GymWalk(const GymWalk<state_size>& other)
:
EnvBase<TimeStep<uint_t>, 
		ScalarDiscreteEnv<state_size, 2, 0, 0>
	   >(other),
with_rest_api_mixin<TimeStep<uint_t>>(other)
{}

template<uint_t state_size>
typename GymWalk<state_size>::dynamics_t
GymWalk<state_size>::build_dynamics_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;
    json j = json::parse(str_response);

    auto dynamics = j["dynamics"];
    return dynamics;
}

template<uint_t state_size>
typename GymWalk<state_size>::time_step_type
GymWalk<state_size>::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return GymWalk::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}

template<uint_t state_size>
void
GymWalk<state_size>::make(const std::string& version,
           const std::unordered_map<std::string, std::any>& options){
			   
	if(this->is_created()){
        return;
    }
	
	const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["version"] = version;
	j["cidx"] = this -> cidx();
    auto body = j.dump();

    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

	this->set_version_(version);
    this->make_created_();
}

template<uint_t state_size>
typename GymWalk<state_size>::time_step_type
GymWalk<state_size>::step(const action_type&  action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created() && "Environment has not been created");
#endif

	if(this->get_current_time_step_().last()){
         return this->reset(42, std::unordered_map<std::string, std::any>());
     }

 const auto request_url = std::string(this->get_url()) + "/step";
    http::Request request{request_url};

    auto copy_idx = this -> cidx();
   
	using json = nlohmann::json;
    json j;
	j["cidx"] = copy_idx;
	j["action"] = action;
    const auto response = request.send("POST", j.dump());

    if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();
}

template<uint_t state_size>
bool
GymWalk<state_size>::is_alive()const{
	
	
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

template<uint_t state_size>
bool
GymWalk<state_size>::close(){

     if(!this->is_created()){
        return;
    }

	auto copy_idx = this -> cidx();
	using json = nlohmann::json;
    json j;
	j["cidx"] = copy_idx;
	
	auto url = this -> get_url();
	
    http::Request request{url + "/close"};
    const auto response = request.send("POST");
    this -> invalidate_is_created_flag_();

}

template<uint_t state_size>
typename GymWalk<state_size>::time_step_type
GymWalk<state_size>::reset(uint_t seed,
						   const std::unordered_map<std::string, std::any>& /*options*/){

    if(!this->is_created()){
#ifdef RLENVSCPP_DEBUG
     assert(this->is_created() && "Environment has not been created");
#endif
     return time_step_type();
    }
	
	auto copy_idx = this -> cidx();
	
	auto url = this -> get_url();
    const auto request_url = url + "/reset";
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

    this->create_time_step_from_response_(response);
    return this -> get_current_time_step_();
}

template<uint_t state_size>
GymWalk<state_size>
GymWalk<state_size>::make_copy(uint_t cidx)const{
	
	auto api_base_url = this -> get_api_url();
	GymWalk<state_size> copy(api_base_url,cidx);
	std::unordered_map<std::string, std::any> ops;
	auto version = this -> version();
	copy.make(version, ops);
	return copy;
}

}
}
}

#endif // GYM_WALK_H
