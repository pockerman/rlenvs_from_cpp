#include "rlenvs/envs/gymnasium/toy_text/frozen_lake_env.h"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/envs/time_step_type.h"
#include "rlenvs/extern/nlohmann/json/json.hpp"


#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <memory>

namespace rlenvscpp{
namespace envs{
namespace gymnasium{

template<uint_t side_size>
const std::string FrozenLake<side_size>::name = "FrozenLake";

template<uint_t side_size>
FrozenLake<side_size>::FrozenLake(const std::string& api_base_url)
:
ToyTextEnvBase<TimeStep<uint_t>,
			   frozenlake_state_size<side_size>::size, 
			   4>(0, "FrozenLake", api_base_url,"/gymnasium/frozen-lake-env"),
is_slippery_(true)
{}
 
template<uint_t side_size>
FrozenLake<side_size>::FrozenLake(const std::string& api_base_url, 
		                          const uint_t cidx, bool slippery)
		   :
ToyTextEnvBase<TimeStep<uint_t>,
			   frozenlake_state_size<side_size>::size, 
			   4>(cidx, "FrozenLake", api_base_url, "/gymnasium/frozen-lake-env"),
is_slippery_(slippery)
{}			
			   
template<uint_t side_size>
typename FrozenLake<side_size>::dynamics_t
FrozenLake<side_size>::build_dynamics_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;
    json j = json::parse(str_response);

    auto dynamics = j["dynamics"];
    return dynamics;
}

template<uint_t side_size>
typename FrozenLake<side_size>::time_step_type
FrozenLake<side_size>::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"].template get<uint_t>();
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return FrozenLake<side_size>::time_step_type(TimeStepEnumUtils::time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}

template<uint_t side_size>
void
FrozenLake<side_size>::make(const std::string& version,
                            const std::unordered_map<std::string, std::any>& options){

    if(this->is_created()){
        return;
    }

    is_slippery_ = true;

    auto slip_itr = options.find("is_slippery");
    if( slip_itr != options.end()){
        is_slippery_ = std::any_cast<bool>(slip_itr->second);
    }
	
	auto copy_idx = this -> cidx();
    const auto request_url = std::string(this->get_url()) + "/make";
    http::Request request{request_url};

    using json = nlohmann::json;
    json j;
    j["version"] = version;
    j["map_name"] = map_type();
    j["is_slippery"]  = is_slippery_;
	j["cidx"] = copy_idx;

    auto body = j.dump();
    const auto response = request.send("POST", body);

    if(response.status.code != 201){
        throw std::runtime_error("Environment server failed to create Environment");
    }

    this->set_version_(version);
    this->make_created_();
}

template<uint_t side_size>
typename FrozenLake<side_size>::time_step_type
FrozenLake<side_size>::step(const action_type& action){

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
	auto body = j.dump();

    const auto response = request.send("POST", body);

    if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to step environment");
    }

    this->get_current_time_step_() = this->create_time_step_from_response_(response);
    return this->get_current_time_step_();

}

template<uint_t side_size>
std::unique_ptr<typename FrozenLake<side_size>::base_type> 
FrozenLake<side_size>::make_copy(uint_t cidx)const{
	
	auto api_base_url = this -> get_api_url();
	auto slippery = this -> is_slippery();
	
	auto ptr = std::unique_ptr<FrozenLake<side_size>>(new FrozenLake(api_base_url,
												                     cidx,
																	 slippery));
	
	std::unordered_map<std::string, std::any> ops;
	ops["is_slippery"] = this -> is_slippery();
	auto version = this -> version();
	ptr -> make(version, ops);
	return ptr;
												
}

template class FrozenLake<4>;
template class FrozenLake<8>;

}
}
}
