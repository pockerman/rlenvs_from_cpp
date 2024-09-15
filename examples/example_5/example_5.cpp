
#include "rlenvs/envs/grid_world/grid_world_env.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/rlenvs_consts.h"


#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

#include <iostream>
#include <string>
#include <random>


namespace example_5{



}

int main(){

    rlenvs_cpp::envs::grid_world::Gridworld<4> env;
    std::cout<<"Environment name: "<<env.name<<std::endl;

    // make the environment
    std::unordered_map<std::string, std::any> options;
    env.make("v0", options);


    /*auto time_step = env.reset(42, std::unordered_map<std::string, std::any>());

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;
    std::cout<<"Time step "<<time_step<<std::endl;*/

   // std::cout<<"Trajectory size: "<<trajectory.size()<<std::endl;

    // finally close the environment
    env.close();
    return 0;
}
