#ifndef ALE_ADVENTURE_ENV_H
#define ALE_ADVENTURE_ENV_H


#include "rlenvs/rlenvscpp_config.h"

#ifdef USE_ALE_ENS

#include "rlenvs/discrete_space.h"
#include "rlenvs/time_step.h"

#include <ale/ale_interface.hpp>
#include <boost/noncopyable.hpp>

namespace rlenvs_cpp{
namespace envs{
namespace rlenvsale{
	
	
BETTER_ENUM(ALEAdventureActionsEnum, int,
            ZERO=0, ONE=1, TWO=2, UP=3, INVALID_ACTION=4);

///
/// \brief The ALEAdventure class. Wrapper to the 
/// ALE Adventure environment: https://ale.farama.org/environments/adventure/
///
class ALEAdventure: private boost::noncopyable
{
public:
		
	static const std::string name;
		
	///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<18> action_space_type;
		
	///
	/// \brief Constructor
	///
	ALEAdventure();
	
	
	///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    void make(const std::string& /*version*/="v0",
			  const std::unordered_map<std::string, std::any>& /*options*/=std::unordered_map<std::string, std::any>());
	
	///
	/// \brief n_actions. Returns the number of actions
	///
	uint_t n_actions()const noexcept{return action_space_type::size;}
	
	
private:
	
	///
	/// \brief the ALE interface
	///
	ale::ALEInterface ale_;
		
		
		
	
};


		
}
}
}


#endif
#endif