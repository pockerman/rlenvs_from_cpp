#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/toy_text/frozen_lake_env.h"
#include "rlenvs/envs/gymnasium/toy_text/taxi_env.h"
#include "rlenvs/envs/gymnasium/toy_text/black_jack_env.h"
#include "rlenvs/envs/gymnasium/toy_text/cliff_world_env.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <any>

namespace example_1{

const std::string SERVER_URL = "http://0.0.0.0:8001/api";

void test_frozen_lake(){

    rlenvscpp::envs::gymnasium::FrozenLake<4> env(SERVER_URL);

    std::cout<<"Environame URL: "<<env.get_url()<<std::endl;

    // make the environment
    std::unordered_map<std::string, std::any> options;
    options.insert({"is_slippery", false});
    env.make("v1", options);

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;
    std::cout<<"Number of valid actions? "<<env.n_actions()<<std::endl;
    std::cout<<"Number of states? "<<env.n_states()<<std::endl;

    // reset the environment
    auto time_step = env.reset(42, std::unordered_map<std::string, std::any>());

    std::cout<<"Reward on reset: "<<time_step.reward()<<std::endl;
    std::cout<<"Observation on reset: "<<time_step.observation()<<std::endl;
    std::cout<<"Is terminal state: "<<time_step.done()<<std::endl;

    //...print the time_step
    std::cout<<time_step<<std::endl;

    // take an action in the environment
	// 2 = RIGHT
    auto new_time_step = env.step(2);

    std::cout<<new_time_step<<std::endl;

    // get the dynamics of the environment for the given state and action
    auto state = 0;
    auto action = 1;
    auto dynamics = env.p(state, action);

    std::cout<<"Dynamics for state="<<state<<" and action="<<action<<std::endl;

    for(auto item:dynamics){

        std::cout<<std::get<0>(item)<<std::endl;
        std::cout<<std::get<1>(item)<<std::endl;
        std::cout<<std::get<2>(item)<<std::endl;
        std::cout<<std::get<3>(item)<<std::endl;
    }

    // synchronize the environment
    env.sync(std::unordered_map<std::string, std::any>());
	
	auto copy_env_ptr = env.make_copy(1);
	copy_env_ptr -> reset();
	
	std::cout<<"Org env cidx: "<<env.cidx()<<std::endl;
	std::cout<<"Copy env cidx: "<<copy_env_ptr -> cidx()<<std::endl;
	
	copy_env_ptr -> close();

    // close the environment
    env.close();

}

void test_taxi(){

    rlenvscpp::envs::gymnasium::Taxi env(SERVER_URL);

    std::cout<<"Environame URL: "<<env.get_url()<<std::endl;

    // make the environment
    std::unordered_map<std::string, std::any> options;
    env.make("v3", options);

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;
    std::cout<<"Number of valid actions? "<<env.n_actions()<<std::endl;
    std::cout<<"Number of states? "<<env.n_states()<<std::endl;
	
	
    // reset the environment
    auto time_step = env.reset(42, std::unordered_map<std::string, std::any>());

    std::cout<<"Reward on reset: "<<time_step.reward()<<std::endl;
    std::cout<<"Observation on reset: "<<time_step.observation()<<std::endl;
    std::cout<<"Is terminal state: "<<time_step.done()<<std::endl;

    //...print the time_step
    std::cout<<time_step<<std::endl;

    // take an action in the environment
	// move RIGHT
    auto new_time_step = env.step(2);

    std::cout<<new_time_step<<std::endl;

    // get the dynamics of the environment for the given state and action
    auto state = 0;
    auto action = 1;
    auto dynamics = env.p(state, action);

    std::cout<<"Dynamics for state="<<state<<" and action="<<action<<std::endl;

    for(auto item:dynamics){

        std::cout<<std::get<0>(item)<<std::endl;
        std::cout<<std::get<1>(item)<<std::endl;
        std::cout<<std::get<2>(item)<<std::endl;
        std::cout<<std::get<3>(item)<<std::endl;
    }
	
	
    // close the environment
    env.close();

}


void test_black_jack(){

    rlenvscpp::envs::gymnasium::BlackJack env(SERVER_URL);
    std::unordered_map<std::string, std::any> options;
    options["natural"] = true;

    std::cout<<"Environment created..."<<std::endl;
    env.make("v1", options);

    std::cout<<"Environment reset..."<<std::endl;
    auto state = env.reset();

    std::cout<<"Environment step..."<<std::endl;
	
	// 0 = HIT
	// 1 = STICK
    env.step(0);
    env.step(1);

    // synchronize the environment
    env.sync(std::unordered_map<std::string, std::any>());

    // close the environment
    env.close();

}


void test_cliff_world(){

	rlenvscpp::envs::gymnasium::CliffWorld env(SERVER_URL);

    std::cout<<"Environment URL: "<<env.get_url()<<std::endl;

    // make the environment
    std::unordered_map<std::string, std::any> options;
	options["max_episode_steps"] = std::any(static_cast<rlenvscpp::uint_t>(10));
    env.make("v0", options);

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;
    std::cout<<"Number of valid actions? "<<env.n_actions()<<std::endl;
    std::cout<<"Number of states? "<<env.n_states()<<std::endl;

    // reset the environment
    auto time_step = env.reset(42, std::unordered_map<std::string, std::any>());

    std::cout<<"Reward on reset: "<<time_step.reward()<<std::endl;
    std::cout<<"Observation on reset: "<<time_step.observation()<<std::endl;
    std::cout<<"Is terminal state: "<<time_step.done()<<std::endl;

    //...print the time_step
    std::cout<<time_step<<std::endl;

    // take an action in the environment
	// 0 = UP
    auto new_time_step = env.step(0);

    std::cout<<new_time_step<<std::endl;

    // get the dynamics of the environment for the given state and action
    auto state = 0;
    auto action = 1;
    auto dynamics = env.p(state, action);

    std::cout<<"Dynamics for state="<<state<<" and action="<<action<<std::endl;

    for(auto item:dynamics){

        std::cout<<std::get<0>(item)<<std::endl;
        std::cout<<std::get<1>(item)<<std::endl;
        std::cout<<std::get<2>(item)<<std::endl;
        std::cout<<std::get<3>(item)<<std::endl;
    }

    // synchronize the environment
    env.sync(std::unordered_map<std::string, std::any>());

    // close the environment
    env.close();

}

}


int main(){


    std::cout<<"Testing FrozenLake..."<<std::endl;
    example_1::test_frozen_lake();
    std::cout<<"===================="<<std::endl;
    //std::cout<<"Testing Taxi..."<<std::endl;
    //example_1::test_taxi();
    //std::cout<<"===================="<<std::endl;
    //std::cout<<"Testing BlackJack..."<<std::endl;
    //example_1::test_black_jack();
    //std::cout<<"===================="<<std::endl;
    //std::cout<<"Testing CliffWorld..."<<std::endl;
    //example_1::test_cliff_world();
    //std::cout<<"===================="<<std::endl;
    return 0;
}
