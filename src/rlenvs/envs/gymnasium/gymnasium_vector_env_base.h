/**
  * Base class for Gymnasium vector environments.
  * See: https://gymnasium.farama.org/api/vector/sync_vector_env/
  * 
  *
*/


#ifndef GYMNASIUM_VECTOR_ENV_BASE_H
#define GYMNASIUM_VECTOR_ENV_BASE_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/envs/gymnasium/gymnasium_env_base.h"
#include "rlenvs/rlenvs_types_v2.h"


#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <stdexcept>
	

namespace rlenvscpp{
namespace envs{
namespace gymnasium {
	
	
template<typename VectorTimeStepType>
class GymnasiumVecEnvBase: public GymnasiumEnvBase<VectorTimeStepType>
{
	
public:
	
	
	///
    /// \brief make. Builds the environment. Throws an exception
	/// if the number of environments is not specified
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) = 0;
					  
					  
	///
	/// \brief Returns the number of environments
	///
	uint_t get_n_envs()const noexcept {return n_envs_;}
	
	///
	/// \brief 
	///
	void reset_if_any_done(bool flag)noexcept{reset_if_any_done_ = flag;}
	
	///
	/// \brief 
	///
	bool get_reset_if_any_done()const noexcept{return reset_if_any_done_;}
	
protected:
	
	///
    /// \brief Constructor
    ///
    GymnasiumVecEnvBase(const std::string& url);
					  
private:
	
	
	uint_t n_envs_{0};
	
	///
	/// \brief Flag indicating that the vector environment
	/// is reset if any of the environments is done
	///
	bool reset_if_any_done_{false};
	
};


template<typename VectoTimeStepType>
GymnasiumVecEnvBase<VectoTimeStepType>::GymnasiumVecEnvBase(const std::string& url)
:
GymnasiumEnvBase<VectoTimeStepType>(url)
{}

template<typename VectoTimeStepType>
void 
GymnasiumVecEnvBase<VectoTimeStepType>::make(const std::string& version,
                                             const std::unordered_map<std::string, std::any>& options){

	auto reset_if_any_done_itr = options.find("reset_if_any_done");
	
	if(reset_if_any_done_itr != options.end()){
		reset_if_any_done_ = std::any_cast<bool>(reset_if_any_done_itr->second);
	}
	
	auto n_envs_itr = options.find("num_envs");
	
	if(n_envs_itr != options.end()){
		n_envs_ = std::any_cast<uint_t>(n_envs_itr->second);
		return;
	}
	
	throw std::logic_error("num_envs variable is not provided");
												 
}
	
}
}
}


#endif // GYMNASIUMENVBASE_H
