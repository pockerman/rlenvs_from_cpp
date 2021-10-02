# gym_from_cpp

Some utilities based on <a href="https://www.boost.org/doc/libs/1_76_0/libs/python/doc/html/tutorial/index.html">Boost.Python</a> to use 
<a href="#">OpenAI-Gym</a> environments from C++ drivers. This repository does not aim at executing general Python code from C++ drivers. Instead, it focuses
on simply facilitating interaction with  OpenAI-Gym environments. If you require something a lot more general, have a look at <a href="https://github.com/pybind/pybind11">pybind11</a>.

Thus, ```gym_from_cpp``` uses Boost.Python heavily under the hoods.  Moreover,
it does not attempt to hide any of the  functionality provided by  Boost.Python (see basic example below). 

The various wrappers of the environments exposed
should abide with <a href="https://github.com/deepmind/dm_env/blob/master/docs/index.md">dm_env</a> specification.


## Basic example

```
#include "gymfcpp/frozen_lake.h"
#include <boost/python.hpp>
#include <iostream>

int main(){

    try
    {
    	// initialize the interpreter
        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        
        gymfcpp::FrozenLake env("v0", gym_namespace, false);

	// make slipery
        env.make(true);

	// reset the environment
        auto step = env.reset();
        std::cout<<step<<std::endl;

	// step and compute any extra
        step = env.step(1, true);
        std::cout<<step<<std::endl;

        std::cout<<"Step with prob="<<step.get_extra<gymfcpp::real_t>("prob")<<std::endl;

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



## Environments implemented

- ```FrozenLake```
- ```Blackjack```
- ```CliffWalking```
- ```CartPole```

## Dependencies

- A compiler that supports C++20 e.g. g++10
- Boost.Python
- OpenAI-Gym installed
- CMake

## Issues

### ```pyconfig.h``` not found

- Run: ```find /usr/include -name pyconfig.h```
- Export: ```export CPLUS_INCLUDE_PATH``` for example ```export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/usr/include/python3.8/"```

