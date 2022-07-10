#include "gymfcpp/taxi_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/render_mode_enum.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>
//#include <boost/python/numpy.hpp>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::gymfcpp::Taxi;

}


TEST(TestTaxi, TestConstructor) {

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        Taxi env("v3", main_namespace);
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Failed to create environment instance...";
    }

}


TEST(TestTaxi, Test_Make)
{

    try{

        Py_Initialize();
        //boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        Taxi env("v3", main_namespace, false);
        env.make();


        ASSERT_TRUE(env.is_created);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not make the environment";
    }
}


TEST(TestTaxi, Test_Reset)
{

    try{

        Py_Initialize();
        //boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        Taxi env("v3", main_namespace, false);
        env.make();

        auto state = env.reset();
        ASSERT_TRUE(state.first());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not reset the environment";
    }
}

TEST(TestMountainCar, Test_Step)
{

    try{

        Py_Initialize();
        //boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        Taxi env("v3", main_namespace, false);
        env.make();
        env.reset();

        auto step_result = env.step(0);
        ASSERT_TRUE(step_result.mid());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}


TEST(TestMountainCar, Test_Render)
{

    try{

        Py_Initialize();
        //boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        Taxi env("v3", main_namespace, false);
        env.make();
        env.reset();

        env.render(rlenvs_cpp::RenderModeType::human);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not render the environment";
    }
}



