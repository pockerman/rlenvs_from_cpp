# rlenvs_from_cpp

```rlenvs_from_cpp``` is an effort to provide implementations and wrappers of environments for reinforcement learning algorithms to be used by C++ drivers. 
Currently, we provide a minimal number of wrappers for some common OpenAI-Gym environments. Namely

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

Using OpenAI-Gym requires <a href="https://www.boost.org/doc/libs/1_76_0/libs/python/doc/html/tutorial/index.html">Boost.Python</a> as well as
<a href="https://github.com/openai/gym">OpenAI-Gym</a> package installed. See below for more dependencies.

 
## Basic example

```
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/frozen_lake_env.h"
#include <boost/python.hpp>
#include <iostream>

int main(){

    try
    {
    	Py_Initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");
        rlenvs_cpp::gymfcpp::FrozenLake<4> env("v1", main_namespace, false);

        env.make();

        auto step = env.reset();
        std::cout<<step<<std::endl;

        step = env.step(1, true);
        std::cout<<step<<std::endl;

        std::cout<<"Step with prob="<<step.get_extra<rlenvs_cpp::real_t>("prob")<<std::endl;

    }
    catch(boost::python::error_already_set const &)
    {
        PyErr_Print();
    }

    return 0;
}

================

Step type.... FIRST
Reward..... ..0
Observation.. 0

Step type.... MID
Reward..... ..0
Observation.. 0

Step with prob=0.333333


```

## Dependencies

- A compiler that supports C++20 e.g. g++-11
- <a href="https://www.boost.org/">Boost C++</a> and in particlar Boost.Python
- <a href="https://github.com/Farama-Foundation/Gymnasium">Gymnasium</a> (<a href="https://github.com/openai/gym">OpenAI-Gym</a> development has been switched to Gymnasium)
- <a href="https://cmake.org/">CMake</a> >= 3.6
- <a href="https://github.com/google/googletest">Gtest</a> (if configured with tests)
- <a href="https://pytorch.org/">PyTorch</a> (C++ bindings with C++11 ABI) (if configured with PyTorch)

In addition, the library also incorporates ```(src/extern)```

- <a href="https://github.com/elnormous/HTTPRequest">HTTPRequest</a>
- <a href="http://github.com/aantron/better-enums">better-enums</a>

## Installation

The usual CMake based installation process is used. Namely

```
mkdir build && cd build && cmake ..
make install
```

## Generate documentation

There are extra dependencies if you want to generate the documentation. Namely,

- Doxygen
- Sphinx
- sphinx_rtd_theme
- breathe
- m2r2

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

