#ifndef TAXI_H
#define TAXI_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/toy_text/toy_text_base.h"
#include "rlenvs/envs/time_step.h"
//#include "rlenvs/envs/space_type.h"
#include "rlenvs/extern/HTTPRequest.hpp"


#include <map>
#include <string>
#include <any>
#include <unordered_map>
#include <memory>


namespace rlenvscpp{
namespace envs {
namespace gymnasium{

///
/// \brief The Taxi class
///
class Taxi final: public  ToyTextEnvBase<TimeStep<uint_t>, 500, 6>
{
public:
	
	///
    /// \brief name
    ///
    static  const std::string name;
	
    ///
	/// \brief The base type
	///
	typedef typename ToyTextEnvBase<TimeStep<uint_t>, 500, 6>::base_type base_type;
	
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
    /// \brief Taxi
    ///
    Taxi(const std::string& api_base_url);
	
    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    ~Taxi()=default;

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& /*options*/) override final;
					  
	
	///
    /// \brief step
    ///
    virtual time_step_type step(const action_type& action) override final;

    ///
	/// \brief Create a new copy of the environment with the given
	/// copy index
	///
	virtual std::unique_ptr<base_type> make_copy(uint_t cidx)const override final;

protected:
	
	///
	/// \brief Constructor
	///
	Taxi(const std::string& api_base_url, 
		 const uint_t cidx);

     ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const override final;

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;



};

}
}
}
#endif // TAXI_H
