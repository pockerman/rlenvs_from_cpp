#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/classic_control/vector/acrobot_vec_env.h"
#include "rlenvs/rlenvs_consts.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <any>
#include <vector>
#include <ios>


int main(){

	using namespace rlenvs_cpp::envs::gymnasium;
	using rlenvs_cpp::uint_t;
	
	const std::string url = "http://0.0.0.0:8001/api";
    
	// Acrobot vector environment
	AcrobotV env(url);
	
	std::cout<<"Name: "<<env.name<<std::endl;
	std::cout<<"Number of actions: "<<env.n_actions()<<std::endl;
	
	std::unordered_map<std::string, std::any> options;
	options["num_envs"] = std::any(static_cast<uint_t>(3));
	
	// make the environment
	env.make("v1", options);
	
	std::cout<<"Reseting the environment... "<<std::endl;
	auto time_step = env.reset();
	//std::cout<<"Time step: "<<time_step<<std::endl;
	
	std::cout<<"Acting on the environment... "<<std::endl;
	// step in the environment
	std::vector<uint_t> actions(3, 0);
	actions[1] = 1;
	actions[2] = 2;
	time_step = env.step(actions);
	
	//std::cout<<"Time step after action: "<<time_step<<std::endl;
	
	std::cout<<"Closing the environment... "<<std::endl;
	env.close();
	
	std::cout<<"Is active? "<<env.is_alive()<<std::noboolalpha <<std::endl;
    return 0;
}
