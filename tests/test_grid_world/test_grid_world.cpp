#include "gymfcpp/grid_world_env.h"

#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"

#include <gtest/gtest.h>

namespace{

using gymfcpp::uint_t;
using gymfcpp::real_t;

}


TEST(TestGridworld, TestConstructor4x4) {

    try{

        rlenvs::Gridworld<4> env("v0", rlenvs::GridworldInitType::STATIC, false);

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_EQ(env.version(), "v0");
        ASSERT_FALSE(env.is_created());

        // TODO: Think how to test this
        ASSERT_TRUE(rlenvs::to_string(env.init_type()) == rlenvs::to_string(rlenvs::GridworldInitType::STATIC));
        ASSERT_EQ(env.name, "Gridworld");

    }
    catch(...)
    {
        FAIL()<<"Unknown exception thrown";
    }
}


TEST(TestGridworld, TestMake) {

    try{

        rlenvs::Gridworld<4> env("v0", rlenvs::GridworldInitType::STATIC, false);
        env.make();

        ASSERT_TRUE(env.is_created());
        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

    }
    catch(...)
    {
        FAIL()<<"Unknown exception thrown";
    }
}

TEST(TestGridworld, TestStepInvalidMove)
{

    try{

        rlenvs::Gridworld<4> env("v0", rlenvs::GridworldInitType::STATIC, false);
        env.make();

        // make sure we have the right world
        ASSERT_TRUE(env.is_created());
        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

        EXPECT_DEATH(env.step(5), "Invalid move");
    }
    catch(...)
    {
        FAIL()<<"Unknown exception thrown";
    }
}


TEST(TestGridworld, TestStepValidMove)
{

    try{

        rlenvs::Gridworld<4> env("v0", rlenvs::GridworldInitType::STATIC, false);
        env.make();

        // make sure we have the right world
        ASSERT_TRUE(env.is_created());
        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

        env.step(1);

    }
    catch(...)
    {
        FAIL()<<"Unknown exception thrown";
    }
}

/*
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
*/
