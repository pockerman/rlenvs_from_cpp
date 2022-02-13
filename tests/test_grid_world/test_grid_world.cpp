#include "gymfcpp/frozen_lake_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>

namespace{

using gymfcpp::uint_t;
using gymfcpp::real_t;

}


TEST(TestFrozenLake, TestConstructor4x4) {

    try{

        Py_Initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");
        gymfcpp::FrozenLake<4> env("v0", main_namespace);

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_EQ(env.map_type(), "4x4");

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}

TEST(TestFrozenLake, TestConstructor8x8) {

    try{

        Py_Initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");
        gymfcpp::FrozenLake<8> env("v0", main_namespace);

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(64));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_EQ(env.map_type(), "8x8");

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

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::FrozenLake<4> env("v0", main_namespace, false);
        env.make();

        //ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        //ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}


TEST(TestFrozenLake, Test_Reset)
{

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::FrozenLake<4> env("v0", main_namespace, false);
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

TEST(TestFrozenLake, Test_Step)
{

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::FrozenLake<4> env("v0", main_namespace, false);
        env.make();

        auto step_result = env.step(0);
        ASSERT_TRUE(step_result.mid());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestFrozenLake, Test_Step_With_Query)
{

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::FrozenLake<4> env("v0", main_namespace, false);
        env.make();

        auto step_result = env.step(0, true);
        ASSERT_TRUE(step_result.mid());
        ASSERT_DOUBLE_EQ(step_result.get_extra<real_t>("prob"), 0.3333333333333333);
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}


TEST(TestFrozenLake, Test_Get_Dynamics)
{

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::FrozenLake<4> env("v0", main_namespace, false);
        env.make();

        auto dynamics = env.p(1, 3);

        ASSERT_EQ(dynamics.size(), static_cast<uint_t>(3) );

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestFrozenLake, TestRender)
{

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::FrozenLake<4> env("v0", main_namespace, false);
        env.make();
        env.reset();

        env.render(gymfcpp::RenderModeType::human);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

