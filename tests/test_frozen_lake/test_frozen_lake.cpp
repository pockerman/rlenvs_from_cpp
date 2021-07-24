#include "gymfcpp/frozen_lake.h"
#include "gymfcpp/py_interpreter.h"
#include "gymfcpp/types.h"
#include <gtest/gtest.h>
#include <boost/python.hpp>

namespace{

using gymfcpp::uint_t;

}


TEST(TestFrozenLake, TestConstructor) {

    try{

        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        gymfcpp::FrozenLake env("v0", gym_namespace);
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }

}


TEST(TestFrozenLake, Test_Not_Created_Assert_Is_Thrown)
{

    try{

        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        gymfcpp::FrozenLake env("v0", gym_namespace, false);

        ASSERT_DEATH(env.n_states(), "Environment has not been created");

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}

TEST(TestFrozenLake, Test_Make)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        gymfcpp::FrozenLake env("v0", gym_namespace, false);
        env.make();

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}
