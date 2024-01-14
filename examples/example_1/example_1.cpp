#include "rlenvs/rlenvs_types.h"
#include "rlenvs/envs/gymnasium/frozen_lake_env.h"
#include <iostream>
#include <string>


int main(){

    const std::string SERVER_URL = "http://0.0.0.0:8001/api";
    rlenvs_cpp::envs::gymnasium::FrozenLake<4> env(SERVER_URL);

    // make the environment
    env.make("v1", true);

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;;

    // reset the environment
    auto time_step = env.reset();

    std::cout<<"Reward on reset: "<<time_step.reward()<<std::endl;
    std::cout<<"Observation on reset: "<<time_step.observation()<<std::endl;
    std::cout<<"Is terminal state: "<<time_step.done()<<std::endl;

    // let's print the time_step
    std::cout<<time_step<<std::endl;

    // take an action in the environment
    auto new_time_step = env.step(rlenvs_cpp::envs::gymnasium::FrozenLakeActionsEnum::RIGHT);

    std::cout<<new_time_step<<std::endl;

    // close the environment
    env.close();

    std::cout<<"Finilize..."<<std::endl;
    return 0;
}
