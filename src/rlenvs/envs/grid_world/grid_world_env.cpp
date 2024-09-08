// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#include "rlenvs/envs/grid_world/grid_world_env.h"
#include <string>

namespace rlenvs_cpp{
namespace envs{
namespace grid_world{

GridWorldInitType
from_string(const std::string& gw_init_type){

    if(gw_init_type == "STATIC"){
        return rlenvs_cpp::envs::grid_world::GridWorldInitType::STATIC;
    }
    else if(gw_init_type == "RANDOM"){
        return rlenvs_cpp::envs::grid_world::GridWorldInitType::RANDOM;
    }
    else if(gw_init_type == "PLAYER"){
        return rlenvs_cpp::envs::grid_world::GridWorldInitType::PLAYER;
    }

    return rlenvs_cpp::envs::grid_world::GridWorldInitType::INVALID_TYPE;

}

}
}
}
