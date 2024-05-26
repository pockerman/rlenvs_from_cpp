
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gym_pybullet_drones//quadcopter_sim_env.h"
#include "rlenvs/envs/envs_utils.h"
#include "rlenvs/rlenvscpp_config.h"

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>
#include <string>
#include <random>


namespace example_4{

using rlenvs_cpp::uint_t;
const std::string SERVER_URL = "http://0.0.0.0:8001/api";
const uint_t MAX_TRAJECTORY_SIZE = 10;

typedef rlenvs_cpp::envs::gym_pybullet_drones::QuadcopterSimEnv env_type;


auto random_action_selector = [](auto /*state*/){

    // randomly select an action
     std::mt19937 gen(42); // mersenne_twister_engine seeded with rd()
     std::uniform_int_distribution<> distrib(0, 3);
     auto action = distrib(gen);
     return action;

};

}

int main(){

    using namespace example_4;

    env_type env(SERVER_URL, "QuadcopterSimEnv");

    std::cout<<"Environame URL: "<<env.get_url()<<std::endl;
    std::cout<<"Environment name: "<<env.env_name()<<std::endl;

    // make the environment
    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);
    auto time_step = env.reset(42, std::unordered_map<std::string, std::any>());

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;
    std::cout<<"Time step "<<time_step<<std::endl;

   // std::cout<<"Trajectory size: "<<trajectory.size()<<std::endl;

    // finally close the environment
    env.close();
    return 0;
}
