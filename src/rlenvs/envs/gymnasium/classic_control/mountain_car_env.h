#ifndef MOUNTAIN_CAR_H
#define MOUNTAIN_CAR_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/gymnasium_env_base.h"
#include "rlenvs/envs/time_step.h"
#include "rlenvs/envs/env_types.h"


#include <string>
#include <vector>
#include <tuple>
#include <any>
#include <unordered_map>

namespace rlenvscpp{
namespace envs{
namespace gymnasium{

	/*
BETTER_ENUM(MountainCarActionsEnum, int, ACCELERATE_LEFT=0, 
										 DO_NOT_ACCELERATE=1, 
										 ACCELERATE_RIGHT=2, 
										 INVALID_ACTION=3);

///
/// \brief The MountainCarData struct. Wrapper for the environment data
///
struct MountainCarData
{
    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<3> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_t
    ///
    typedef ContinuousSpace<3> state_space_type;

    ///
    /// \brief state_t
    ///
    typedef state_space_type::item_t state_type;

    

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;


};
*/

///
/// \brief The MountainCar class
///
class MountainCar final:  public GymnasiumEnvBase<TimeStep<std::vector<real_t>>, 
                                                  ContinuousVectorStateDiscreteActionEnv<3, 2, 0, real_t >
												 >
{

public:
	
	///
    /// \brief name
    ///
    static  const std::string name;

    ///
	/// \brief Base class type
	///
	typedef GymnasiumEnvBase<TimeStep<std::vector<real_t> >,
							 ContinuousVectorStateDiscreteActionEnv< 3, // size of state
							                                         2, // end of action space
																	 0, // start of action space
																	 real_t> // type of state
																	>::base_type base_type;
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
    /// \brief MountainCar. Constructor. Creates an environment.
    /// \param version The version of the environment
    /// \param gym_namespace The boost::python open-ai gym namespace
    /// \param do_create If true it calls make
    ///
    MountainCar(const std::string& api_base_url);

    ///
    /// \brief ~MountainCar. Destructor.
    ///
    ~MountainCar()=default;

    ///
    /// \brief make. Build the environment
    ///
    virtual void make(const std::string& version,
              const std::unordered_map<std::string, std::any>& /*options*/) override final;
			  
	
	///
    /// \brief step
    ///
    virtual time_step_type step(const action_type& action)override final;
	
	///
	/// \brief Create a new copy of the environment with the given
	/// copy index
	///
	virtual std::unique_ptr<base_type> make_copy(uint_t cidx)const override final;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    
protected:
	
	///
	/// \brief Constructor. Protected so that applications
	/// cannot explicitly instantiate copies
	///
	MountainCar(const std::string& api_base_url, 
				const uint_t cidx);

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;



};
}
}
}
#endif // MOUNTAIN_CAR_H
