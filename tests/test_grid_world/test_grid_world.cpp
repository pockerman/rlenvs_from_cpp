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

        // this should always succeed
        env.step(1);

    }
    catch(...)
    {
        FAIL()<<"Unknown exception thrown";
    }
}


TEST(TestGridworld, TestGetObservationFail)
{
    try{

        rlenvs::Gridworld<4> env("v0", rlenvs::GridworldInitType::STATIC, false);

        EXPECT_DEATH(env.get_raw_observation(), "Environment has not been created. Have you called make?");
    }
    catch(...)
    {

        FAIL()<<"Unknown exception thrown";
    }
}


TEST(TestGridworld, TestGetObservation)
{

    try{

        rlenvs::Gridworld<4> env("v0", rlenvs::GridworldInitType::STATIC, false);
        env.make();

        // make sure we have the right world
        ASSERT_TRUE(env.is_created());
        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

        auto state = env.get_raw_observation();

        ASSERT_EQ(state.size(), static_cast<uint_t>(env.n_components));

        for(uint_t c=0; c< env.n_components; ++c){
            ASSERT_EQ(state[c].size(), static_cast<uint_t>(env.side_size));
        }
    }
    catch(...)
    {
        FAIL()<<"Unknown exception thrown";
    }
}


TEST(TestGridworld, TestConstructor4x4Random)
{

    try{

        rlenvs::Gridworld<4> env("v0", rlenvs::GridworldInitType::STATIC, 42, 10.0, false);

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_EQ(env.version(), "v0");
        ASSERT_FALSE(env.is_created());
        ASSERT_TRUE(env.has_random_state());
        ASSERT_EQ(env.seed(), 42);
        ASSERT_EQ(env.noise_factor(), 10.0);

        // TODO: Think how to test this
        ASSERT_TRUE(rlenvs::to_string(env.init_type()) == rlenvs::to_string(rlenvs::GridworldInitType::STATIC));
        ASSERT_EQ(env.name, "Gridworld");

    }
    catch(...)
    {

        FAIL()<<"Unknown exception thrown";
    }
}

/*
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
