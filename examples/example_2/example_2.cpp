#include "gymfcpp/frozen_lake_env.h"

#include <boost/python.hpp>
#include <iostream>
#include <string>

int main(){

    try
    {
        Py_Initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");
        gymfcpp::FrozenLake<4> env("v0", main_namespace, false);

        env.make();

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
