[![Build rlenvs](https://github.com/pockerman/rlenvs_from_cpp/actions/workflows/build.yml/badge.svg)](https://github.com/pockerman/rlenvs_from_cpp/actions/workflows/build.yml)
# rlenvs_from_cpp

```rlenvs_from_cpp``` is an effort to provide implementations and wrappers of environments for reinforcement learning algorithms to be used by C++ drivers. 
Currently, we provide a minimal number of wrappers for some common Gymnasium (former OpenAI-Gym) environments. Namely

- ```FrozenLake``` with ```4x4``` map
- ```FrozenLake``` with ```8x8``` map
- ```Blackjack```
- ```CliffWalking```
- ```CartPole```
- ```MountainCar```
- ```Taxi```
- ```Pendulum```: https://gymnasium.farama.org/environments/classic_control/pendulum/
- ```Acrobot```: https://gymnasium.farama.org/environments/classic_control/acrobot/
- ~```StateAggregationCartPole``` (implements state aggregation for ```CartPole```)~
- ~```SerialVectorEnvWrapper``` a vector wrapper for various environments~

In addition there are wrappers for

- ```GymWalk``` environment from <a href="https://github.com/mimoralea/gym-walk2">gym_walk</a>
- ```gym-pybullet-drones``` from <a href="https://github.com/utiasDSL/gym-pybullet-drones/tree/main">gym-pybullet-drones</a>
- ```GridWorld``` from <a href="https://github.com/DeepReinforcementLearning/DeepReinforcementLearningInAction">Deep Reinforcement Learning In Action</a>
- ```Connect2``` from <a href="https://github.com/JoshVarty/AlphaZeroSimple">AlphaZeroSimple</a> see <a href="examples/example_7/example_7.cpp">example_7</a>

### Vectorised environemtns

There exist some wrappers for vector environments: 

- ```AcrobotV```: <a href="examples/example_8/example_8.cpp">example_8</a>

In general, the environments exposed by the library  should abide with <a href="https://github.com/deepmind/dm_env/blob/master/docs/index.md">dm_env</a> specification.
The following snippet shows how to use the ```FrozenLake```  and ```Taxi``` environments from <a href="https://github.com/Farama-Foundation/Gymnasium/tree/main">Gymnasium</a>.

```cpp
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/toy_text/frozen_lake_env.h"
#include "rlenvs/envs/gymnasium/toy_text/taxi_env.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <any>

namespace example_1{

const std::string SERVER_URL = "http://0.0.0.0:8001/api";

void test_frozen_lake(){

    rlenvs_cpp::envs::gymnasium::FrozenLake<4> env(SERVER_URL);

    std::cout<<"Environame URL: "<<env.get_url()<<std::endl;

    // make the environment
    std::unordered_map<std::string, std::any> options;
    options.insert({"is_slippery", true});
    env.make("v1", options);

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;
    std::cout<<"Number of valid actions? "<<env.n_actions()<<std::endl;
    std::cout<<"Number of states? "<<env.n_states()<<std::endl;

    // reset the environment
    auto time_step = env.reset(42);

    std::cout<<"Reward on reset: "<<time_step.reward()<<std::endl;
    std::cout<<"Observation on reset: "<<time_step.observation()<<std::endl;
    std::cout<<"Is terminal state: "<<time_step.done()<<std::endl;

    //...print the time_step
    std::cout<<time_step<<std::endl;

    // take an action in the environment
    auto new_time_step = env.step(rlenvs_cpp::envs::gymnasium::FrozenLakeActionsEnum::RIGHT);

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

    // synchronize the environment. environment knows how
    // to cast std::any
    env.sync(std::unordered_map<std::string, std::any>());

    // close the environment
    env.close();

}

void test_taxi(){

    rlenvs_cpp::envs::gymnasium::Taxi env(SERVER_URL);

    std::cout<<"Environame URL: "<<env.get_url()<<std::endl;

    // make the environment
    std::unordered_map<std::string, std::any> options;
    env.make("v3", options);

    std::cout<<"Is environment created? "<<env.is_created()<<std::endl;
    std::cout<<"Is environment alive? "<<env.is_alive()<<std::endl;
    std::cout<<"Number of valid actions? "<<env.n_actions()<<std::endl;
    std::cout<<"Number of states? "<<env.n_states()<<std::endl;

    // reset the environment
    auto time_step = env.reset(42);

    std::cout<<"Reward on reset: "<<time_step.reward()<<std::endl;
    std::cout<<"Observation on reset: "<<time_step.observation()<<std::endl;
    std::cout<<"Is terminal state: "<<time_step.done()<<std::endl;

    //...print the time_step
    std::cout<<time_step<<std::endl;

    // take an action in the environment
    auto new_time_step = env.step(rlenvs_cpp::envs::gymnasium::TaxiActionsEnum::RIGHT);

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
}

int main(){


    std::cout<<"Testing FrozenLake..."<<std::endl;
    example_1::test_frozen_lake();
    std::cout<<"===================="<<std::endl;
    std::cout<<"Testing Taxi..."<<std::endl;
    example_1::test_taxi();
    std::cout<<"===================="<<std::endl;
    return 0;
}

```


Some algorithms, such as Monte Carlo, require that we should generate a trajectory, <a href="examples/example_3/example_3.cpp">example 3</a>
shows how to do this. Various RL algorithms using the environments can be found at <a href="https://github.com/pockerman/cuberl/tree/master">cuberl</a>

## How to use

The general use case is to build the library and link it with your driver code to access its functionality.
Furthermore, the ```Gymnasium```, ```gym_pybullet_drones``` environments are accessed via a client/server pattern.
Namely, they are exposed via an API developed using FastAPI.
You need to fire up the server, see dependencies, before using the environments in your code. To do so

```
./start_uvicorn.sh
```

By default the ```uvicorn``` server listents on port 8001. Change this if needed. You can access the OpenAPI specification at

```
http://0.0.0.0:8001/docs
```

Note that currently the implementation is not thread/process safe i.e. if multiple threads/processes access the environment
a global instance of the environment is manipulated. Thus no session based environment exists.

If you need multiple instances of the same environment you can either use one of the exissting vectorised environments (see list above) or 
launch several instances of uvirocrn (listening on different ports). However in this case you need to implement
all the interactions logic yourself as currently no implementation exists to handle such a scenario.


## Dependencies

The library has the following general dependencies

- A compiler that supports C++20 e.g. g++-11
- <a href="https://www.boost.org/">Boost C++</a> 
- <a href="https://cmake.org/">CMake</a> >= 3.6
- <a href="https://github.com/google/googletest">Gtest</a> (if configured with tests)
- <a href="https://eigen.tuxfamily.org/index.php?title=Main_Page">Eigen3</a>

Using the Gymnasium environments requires <a href="https://github.com/Farama-Foundation/Gymnasium/tree/main">Gymnasium</a> installed on your machine.
In addition, you need to install

- <a href="https://fastapi.tiangolo.com/">FastAPI</a>
- <a href="https://www.uvicorn.org/">Uvicorn</a>
- <a href="https://docs.pydantic.dev/latest/">Pydantic</a>


In addition, the library also incorporates, see ```(src/extern)```, the following libraries

- <a href="https://github.com/elnormous/HTTPRequest">HTTPRequest</a>
- <a href="http://github.com/aantron/better-enums">better-enums</a>
- <a href="https://github.com/nlohmann/json">nlohmann/json</a>

There are extra dependencies if you want to generate the documentation. Namely,

- Doxygen
- Sphinx
- sphinx_rtd_theme
- breathe
- m2r2

## Installation

The usual CMake based installation process is used. Namely

```
mkdir build && cd build && cmake ..
make install
```


## Run the tests

You can execute all the tests by running the helper script ```execute_tests.sh```.

## Issues


### Could not find ```boost_system```

It is likely that you are missing the boost_system library with your local Boost installation. This may be the case
is you installed boost via a package manager. On a Ubuntu machine the following should resolve the issue

```
sudo apt-get update -y
sudo apt-get install -y libboost-system-dev
```

