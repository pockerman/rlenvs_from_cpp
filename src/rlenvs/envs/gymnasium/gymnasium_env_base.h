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

namespace rlenvscpp{
namespace envs{
namespace gymnasium {

template<typename TimeStepType, typename SpaceType>
class GymnasiumEnvBase: public EnvBase<TimeStepType, SpaceType>, 
                        public with_rest_api_mixin<TimeStepType>{
public:
	
	
	///
	/// \brief The base_type
	///
	typedef EnvBase<TimeStepType, SpaceType> base_type;

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
	/// \brief The type of the action to be undertaken in the environment
	///
    typedef typename base_type::state_type state_type;
	
	///
	/// \brief Expose the various reset methods we use from base class
	///
	using base_type::reset;

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
	
	/// 
	/// \brief Reset the environment
	///
    virtual time_step_type reset(uint_t seed,
                                 const std::unordered_map<std::string, std::any>& options)override;


protected:

    ///
    /// \brief Constructor
    ///
    GymnasiumEnvBase(const uint_t cidx, 
	                 const std::string& name, 
					 const std::string& api_url,
					 const std::string& resource_path);
					 
	///
    /// \brief build the time step from the server response
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response)const=0;

};

template<typename TimeStepType, typename SpaceType>
GymnasiumEnvBase<TimeStepType, 
                 SpaceType>::GymnasiumEnvBase(const uint_t cidx, const std::string& name, 
											  const std::string& api_url, 
											  const std::string& resource_path)
:
EnvBase<TimeStepType, SpaceType>(cidx, name),
with_rest_api_mixin<TimeStepType>(api_url, resource_path)
{}

template<typename TimeStepType, typename SpaceType>
GymnasiumEnvBase<TimeStepType, SpaceType>::~GymnasiumEnvBase(){
    close();
}

template<typename TimeStepType, typename SpaceType>
bool
GymnasiumEnvBase<TimeStepType, SpaceType>::is_alive()const{
	
	
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

template<typename TimeStepType, typename SpaceType>
void
GymnasiumEnvBase<TimeStepType, SpaceType>::close(){

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

template<typename TimeStepType, typename SpaceType>
typename GymnasiumEnvBase<TimeStepType, SpaceType>::time_step_type
GymnasiumEnvBase<TimeStepType, SpaceType>::reset(uint_t seed,
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



} // gymnasium
} // envs
} // rlenvs_cpp

#endif // GYMNASIUMENVBASE_H
