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
- ```StateAggregationCartPole``` (implements state aggregation for ```CartPole```)
- ```SerialVectorEnvWrapper``` a vector wrapper for various environments

In general, the environments exposed by the library  should abide with <a href="https://github.com/deepmind/dm_env/blob/master/docs/index.md">dm_env</a> specification.
The following is an example how to use the ```FrozenLake``` environment.

```cpp
#include "rlenvs/rlenvs_types_v2.h"
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

    // close the environment
    env.close();

    std::cout<<"Finilize..."<<std::endl;
    return 0;
}
```


## How to use

The general use case is to build the library and link it your driver code to access its functionality.
Furthermore, the Gymnasium environments are accessed via a client/server pattern. Namely, they are exposed via an API developed using FastAPI.
You need to fire up the server, see dependencies, before using the environments in your code. To do so

```cpp
cd rest_api && ./start_uvicorn.sh

```

By default the ```uvicorn``` server listents on port 8001. Change this if needed. You can access the OpenAPI specification at

```
http://0.0.0.0:8001/docs
```


## Dependencies

The library has the following general dependencies

- A compiler that supports C++20 e.g. g++-11
- <a href="https://www.boost.org/">Boost C++</a> 
- <a href="https://cmake.org/">CMake</a> >= 3.6
- <a href="https://github.com/google/googletest">Gtest</a> (if configured with tests)
- <a href="https://pytorch.org/">PyTorch</a> (C++ bindings with C++11 ABI) (if configured with PyTorch)

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

### ```pyconfig.h``` not found

First identify the Python library in your system. You can do so by

```
find /usr/include -name pyconfig.h

```
Update the ```CPLUS_INCLUDE_PATH``` variable to include the headers for your Python library. For example, if you use Python 3.8 then

```
export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/usr/include/python3.8/"
```

### Could not find package ```boost_python```

It is likely that you are missing the boost_python library with your local Boost installation. This may be the case
is you installed boos via a package manager. On a Ubuntu machine the following should resolve the issue

```
sudo apt-get update -y
sudo apt-get install -y libboost-python-dev
```

### Could not find ```boost_system```

This is similar to the issue above. You can resolve it by doing

```
sudo apt-get update -y
sudo apt-get install -y libboost-system-dev
```

