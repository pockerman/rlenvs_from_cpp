
#include "rlenvs/envs/gymnasium/classic_control/pendulum_env.h"
#include "rlenvs/envs/api_server/apiserver.h"
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

	using namespace rlenvscpp::envs::gymnasium;
	using rlenvscpp::envs::RESTApiServerWrapper;
	
	const std::string SERVER_URL = "http://0.0.0.0:8001/api";
    
	RESTApiServerWrapper server(SERVER_URL, true);
	
	Pendulum env(server);
	
	std::cout<<"Name: "<<env.name<<std::endl;
	std::cout<<"Number of actions: "<<env.n_actions()<<std::endl;
	
	// make the environment
	std::unordered_map<std::string, std::any> options;
	env.make("v1", options);
	
	auto time_step = env.reset();
	
	std::cout<<"Time step: "<<time_step<<std::endl;
	
	// step in the environment
	time_step = env.step(0.0);
	
	std::cout<<"Time step after action: "<<time_step<<std::endl;
	
	env.close();
	
	std::cout<<"Is active? "<<env.is_alive()<<std::endl;
	
    return 0;
}
