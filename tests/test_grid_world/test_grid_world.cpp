#include "gymfcpp/grid_world_env.h"

#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"

#include <gtest/gtest.h>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::Gridworld;

}


TEST(TestGridworld, TestConstructor4x4) {

    try{

        Gridworld<4> env("v0", rlenvs_cpp::GridworldInitType::STATIC, false);

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_EQ(env.version(), "v0");
        ASSERT_FALSE(env.is_created());

        // TODO: Think how to test this
        ASSERT_TRUE(rlenvs_cpp::to_string(env.init_type()) == rlenvs_cpp::to_string(rlenvs_cpp::GridworldInitType::STATIC));
        ASSERT_EQ(env.name, "Gridworld");

    }
    catch(...)
    {
        FAIL()<<"Unknown exception thrown";
    }
}


TEST(TestGridworld, TestMake) {

    try{

        Gridworld<4> env("v0", rlenvs_cpp::GridworldInitType::STATIC, false);
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

        Gridworld<4> env("v0", rlenvs_cpp::GridworldInitType::STATIC, false);
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

        Gridworld<4> env("v0", rlenvs_cpp::GridworldInitType::STATIC, false);
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

        Gridworld<4> env("v0", rlenvs_cpp::GridworldInitType::STATIC, false);

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

        Gridworld<4> env("v0", rlenvs_cpp::GridworldInitType::STATIC, false);
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

        Gridworld<4> env("v0", rlenvs_cpp::GridworldInitType::STATIC, 42, 10.0, false);

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_EQ(env.version(), "v0");
        ASSERT_FALSE(env.is_created());
        ASSERT_TRUE(env.has_random_state());
        ASSERT_EQ(env.seed(), 42);
        ASSERT_EQ(env.noise_factor(), 10.0);

        // TODO: Think how to test this
        ASSERT_TRUE(rlenvs_cpp::to_string(env.init_type()) == rlenvs_cpp::to_string(rlenvs_cpp::GridworldInitType::STATIC));
        ASSERT_EQ(env.name, "Gridworld");

    }
    catch(...)
    {

        FAIL()<<"Unknown exception thrown";
    }
}

