#include "gymfcpp/frozen_lake.h"

#include <boost/python.hpp>
#include <iostream>
#include <string>

int main(){

    try
    {
        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        gymfcpp::FrozenLake env("v0", gym_namespace, false);

        env.make(true);

        auto step = env.reset();
        std::cout<<step<<std::endl;

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
