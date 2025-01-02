[![Build rlenvs](https://github.com/pockerman/rlenvs_from_cpp/actions/workflows/build.yml/badge.svg)](https://github.com/pockerman/rlenvs_from_cpp/actions/workflows/build.yml)
# rlenvscpp

```rlenvscpp``` is an effort to provide implementations and wrappers of environments suitable for training reinforcement learning agents
using  C++. In addition, the library provides various utilities such as experiment tracking,
representing trajectories  via waypoints and simple implementation of popular dynamics such as 
quadrotor dynamics.

## Environments

Currently, ```rlenvscpp``` provides the following environments:

| Environment         |   Use REST   | Example                                                   |
| :----------------   | :----------: | :----:                                                    |
| FrozenLake 4x4 map  |   Yes        | <a href="examples/example_1/example_1.cpp">example_1</a>  |
| FrozenLake 8x8 map  |   Yes        | TODO                                                      |
| Blackjack           |   Yes        | <a href="examples/example_1/example_1.cpp">example_1</a>  |
| CliffWalking        |   Yes        | <a href="examples/example_1/example_1.cpp">example_1</a>  |
| CartPole            |   Yes        | TODO                                                      |
| MountainCar         |   Yes        | TODO                                                      |
| Taxi                |   Yes        | <a href="examples/example_1/example_1.cpp">example_1</a>  |
| Pendulum            |   Yes        | <a href="examples/example_6/example_6.cpp">example_6</a>  |
| Acrobot             |   Yes        | TODO                                                      |
| GymWalk             |   Yes        | TODO                                                      |
| gym-pybullet-drones |  TODO        | TODO                                                      |
| GridWorld           |   No         | <a href="examples/example_5/example_5.cpp">example_5</a>  |
| Connect2            |   No         | <a href="examples/example_7/example_7.cpp">example_7</a>  |

The Gymnasium (former OpenAI-Gym) environments utilise a REST API to communicate requests to/from the 
environment and ```rlenvscpp```.

Some environments have a vector implementation meaning multiple instances of the same
environment. Currently, ```rlenvscpp``` provides the following vector environments: 

| Environment         |   Use REST   | Example                                                    |
| :----------------   | :----------: | :----:                                                     |
| AcrobotV            |   Yes        |  <a href="examples/example_8/example_8.cpp">example_8</a>  |

Various RL algorithms using the environments can be found at <a href="https://github.com/pockerman/cuberl/tree/master">cuberl</a>.

### How to use

The following is an example how to use the 
```FrozenLake```   environment from <a href="https://github.com/Farama-Foundation/Gymnasium/tree/main">Gymnasium</a>.

```cpp
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/toy_text/frozen_lake_env.h"
#include "rlenvs/envs/api_server/apiserver.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <any>

namespace example_1{

const std::string SERVER_URL = "http://0.0.0.0:8001/api";

using rlenvscpp::envs::gymnasium::FrozenLake;
using rlenvscpp::envs::RESTApiServerWrapper;


void test_frozen_lake(const RESTApiServerWrapper& server){

    FrozenLake<4> env(server);

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
	
	action = env.sample_action();
	new_time_step = env.step(action);

    std::cout<<new_time_step<<std::endl;
	
    // synchronize the environment
    env.sync(std::unordered_map<std::string, std::any>());
	
	auto copy_env = env.make_copy(1);
	copy_env.reset();
	
	std::cout<<"Org env cidx: "<<env.cidx()<<std::endl;
	std::cout<<"Copy env cidx: "<<copy_env.cidx()<<std::endl;
	
	copy_env.close();

    // close the environment
    env.close();

}

}


int main(){

	using namespace example_1;
	
	RESTApiServerWrapper server(SERVER_URL, true);

    std::cout<<"Testing FrozenLake..."<<std::endl;
    example_1::test_frozen_lake(server);
    std::cout<<"===================="<<std::endl;
    return 0;
}

```

In general, the environments exposed by the library  follow the  <a href="https://github.com/deepmind/dm_env/blob/master/docs/index.md">dm_env</a> specification.
For more details see the <a href="doc/env_spec.md">```rlenvscpp``` environment specification</a> document.

The general use case is to build the library and link it with your driver code to access its functionality.
The environments specified as using REST in the tables above, that is all ```Gymnasium```, ```gym_pybullet_drones``` and ```GymWalk``` 
environments are accessed via a client/server pattern. Namely, they are exposed via an API developed using 
<a href="https://fastapi.tiangolo.com/">FastAPI</a>.
You need to fire up the FastAPI server, see dependencies, before using the environments in your code. 
To do so

```
./start_uvicorn.sh
```

By default the ```uvicorn``` server listents on port 8001. Change this if needed. You can access the OpenAPI specification at

```
http://0.0.0.0:8001/docs
```

Note that currently the implementation is not thread/process safe i.e. if multiple threads/processes access the environment
a global instance of the environment is manipulated. Thus no session based environment exists.
However, you can create copies of the same environment and access this via its dedicate index.
If just one thread/process touches this specific environment you should be ok.
Notice that the FastAPI server only uses a single process to manage all the environments.
In addition, if  you need multiple instances of the same environment you can also  use one 
of the exissting vectorised environments (see table above).

Finally,   you can choose to launch several instances of ```uvirocrn``` (listening on different ports). 
However in this case you need to implement all the interactions logic yourself as currently no implementation exists to handle such a scenario.

## Dynamics 

Apart from the exposed environments, ```rlenvscpp``` exposes classes that 
describe the dynamics of some popular rigid bodies:

| Dynamics            |                       Example                                |
| :----------------   | :----------------------------------------------------------: | 
| Differential drive  |  <a href="examples/example_9/example_9.cpp">example_9</a>    |
| Quadrotor           |  <a href="examples/example_10/example_10.cpp">example_10</a> |

## Miscellaneous

| Item                   |                       Example                                |
| :----------------      | :----------------------------------------------------------: | 
| Environment trajectory |  <a href="examples/example_3/example_3.cpp">example_3</a>    |
| WaypointTrajectory     |  <a href="examples/example_11/example_11.cpp">example_11</a> |

## Dependencies

The library has the following general dependencies

- A compiler that supports C++20 e.g. g++-11
- <a href="https://www.boost.org/">Boost C++</a> 
- <a href="https://cmake.org/">CMake</a> >= 3.10
- <a href="https://github.com/google/googletest">Gtest</a> (if configured with tests)
- <a href="https://eigen.tuxfamily.org/index.php?title=Main_Page">Eigen3</a>

Using the Gymnasium environments requires <a href="https://github.com/Farama-Foundation/Gymnasium/tree/main">Gymnasium</a> installed on your machine.
In addition, you need to install

- <a href="https://fastapi.tiangolo.com/">FastAPI</a>
- <a href="https://www.uvicorn.org/">Uvicorn</a>
- <a href="https://docs.pydantic.dev/latest/">Pydantic</a>

By installing the requirement under ```requirements.txt``` should set your Python environment  up correctly.

In addition, the library also incorporates, see ```(src/extern)```, the following libraries

- <a href="https://github.com/elnormous/HTTPRequest">HTTPRequest</a>
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

You can toggle the following variables

- CMAKE_BUILD_TYPE (default is RELEASE)
- ENABLE_TESTS_FLAG (default is OFF)
- ENABLE_EXAMPLES_FLAG (default is OFF)
- ENABLE_DOC_FLAG (default is OFF)

For example enbling the examples 

```
cmake -DENABLE_EXAMPLES_FLAG=ON ..
make install
```


### Run the tests

You can execute all the tests by running the helper script ```execute_tests.sh```.

### Issues

#### Could not find ```boost_system```

It is likely that you are missing the boost_system library with your local Boost installation. This may be the case
is you installed boost via a package manager. On a Ubuntu machine the following should resolve the issue

```
sudo apt-get update -y
sudo apt-get install -y libboost-system-dev
```

#### FastAPI throws 422 Unpocessable entity

Typically, this is a problem with how the client (400-range error) specified the data
to be sent to the server. 

