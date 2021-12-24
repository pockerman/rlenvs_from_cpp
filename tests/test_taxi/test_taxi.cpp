#include "gymfcpp/taxi.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/render_mode_enum.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace{

using gymfcpp::uint_t;
using gymfcpp::real_t;

}


TEST(TestTaxi, TestConstructor) {

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::Taxi env("v3", main_namespace);
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
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::Taxi env("v3", main_namespace, false);
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
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::Taxi env("v3", main_namespace, false);
        env.make();

        auto state = env.reset();
        ASSERT_TRUE(state.first());

        //auto obs = state.observation();
        //ASSERT_EQ(obs.size(), static_cast<uint_t>(2));

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
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::Taxi env("v3", main_namespace, false);
        env.make();
        env.reset();

        auto step_result = env.step(0);
        ASSERT_TRUE(step_result.mid());

        //auto obs = step_result.observation();
        //ASSERT_EQ(obs, static_cast<uint_t>(327));

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
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::Taxi env("v3", main_namespace, false);
        env.make();
        env.reset();

        env.render(gymfcpp::RenderModeType::human);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not render the environment";
    }
}



