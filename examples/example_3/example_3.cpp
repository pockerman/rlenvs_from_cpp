#include "rlenvs/rlenvs_types.h"
#include "rlenvs/envs/gymnasium/vector/vector_cart_pole.h"

#include <boost/python.hpp>
#include <iostream>
#include <string>

int main(){

    try
    {
        Py_Initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        // create a 4x4 FrozenLake environment
        rlenvs_cpp::envs::gymnasium::vector::VCartPole env("v1", main_namespace, 1, false);

        env.make();

        //auto step = env.reset();
        //std::cout<<step<<std::endl;

        //step = env.step(1, true);
        //std::cout<<step<<std::endl;

        //std::cout<<"Step with prob="<<step.get_extra<rlenvs_cpp::real_t>("prob")<<std::endl;

    }
    catch(boost::python::error_already_set const &)
    {
        PyErr_Print();
    }

    return 0;
}
