#include "rlenvs/rlenvscpp_config.h"

#ifdef USE_ALE_ENS


#include "rlenvs/envs/rlenvsale/ale_adventure_env.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/rlenvs_consts.h"


#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>
#include <string>
#include <random>
#include <unordered_map>

int main(){

	using namespace rlenvs_cpp::envs::rlenvsale;
	
	ALEAdventure env;
	
	std::cout<<"Name: "<<env.name<<std::endl;
	
	env.make();
	/*std::cout<<"Number of actions: "<<env.n_actions()<<std::endl;
	
	// make the environment
	env.make("v1");
	
	auto time_step = env.reset();
	
	std::cout<<"Time step: "<<time_step<<std::endl;
	
	// step in the environment
	time_step = env.step(0.0);
	
	std::cout<<"Time step after action: "<<time_step<<std::endl;
	
	env.close();
	
	std::cout<<"Is active? "<<env.is_alive()<<std::endl;*/
	
    return 0;
}
#else

#include <iostream>

int main(){
	
	std::cout<<"This example requires Farama ALE environments. Rebuild rlenvscpp with ENABLE_ALE_ENVS=ON"<<std::endl;
	return 0;
}
	
#endif
