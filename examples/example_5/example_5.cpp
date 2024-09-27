
#include "rlenvs/envs/grid_world/grid_world_env.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/rlenvs_consts.h"


#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>
#include <string>
#include <random>
#include <unordered_map>


namespace example_5{
	
	using namespace rlenvs_cpp::envs::grid_world;
	
	void create_static(){
		
		std::cout<<"Creating STATIC Gridworld..."<<std::endl;
		
		rlenvs_cpp::envs::grid_world::Gridworld<4> env;

        std::unordered_map<std::string, std::any> options;
        options["mode"] = std::any(rlenvs_cpp::envs::grid_world::to_string(rlenvs_cpp::envs::grid_world::GridWorldInitType::STATIC));

        env.make("v0", options);
		
		std::cout<<"Number of actions: "<<env.n_actions()<<std::endl;
		std::cout<<"Number of states:  "<<env.n_states()<<std::endl;
		
		env.close();
	}
	
	void create_random(){
		
		std::cout<<"Creating RANDOM Gridworld..."<<std::endl;
		
		rlenvs_cpp::envs::grid_world::Gridworld<4> env;

        std::unordered_map<std::string, std::any> options;
        options["mode"] = std::any(rlenvs_cpp::envs::grid_world::to_string(rlenvs_cpp::envs::grid_world::GridWorldInitType::RANDOM));

        env.make("v0", options);
		
		std::cout<<"Number of actions: "<<env.n_actions()<<std::endl;
		std::cout<<"Number of states:  "<<env.n_states()<<std::endl;
		
		env.close();
	}



}

int main(){

	using namespace example_5;
    create_static();
	create_random();
    return 0;
}
