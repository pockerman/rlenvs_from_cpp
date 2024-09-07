#include "rlenvs/envs/gymnasium/toy_text/cliff_world_env.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <any>


namespace{

const std::string SERVER_URL = "http://0.0.0.0:8001/api";
using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::envs::gymnasium::CliffWorld;

}

TEST(TestCliffWorld, TestConstructor) {

    CliffWorld env(SERVER_URL);
    ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

}

TEST(TestCliffWorld, Test_Make)
{

    CliffWorld env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v0", options);
    ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
    ASSERT_EQ(env.n_states(), static_cast<uint_t>(48));
}


TEST(TestCliffWorld, Test_Reset)
{
    CliffWorld env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v0", options);

    auto state = env.reset();
    ASSERT_TRUE(state.first());
}

TEST(TestCliffWorld, Test_Step)
{
    CliffWorld env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v0", options);

    auto step_result = env.step(0);
    ASSERT_TRUE(step_result.mid());

}

TEST(TestCliffWorld, Test_Step_With_Query)
{
    CliffWorld env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v0", options);

    auto step_result = env.step(0);
    ASSERT_TRUE(step_result.mid());

    ASSERT_DOUBLE_EQ(step_result.get_extra<real_t>("prob"), 0.3333333333333333);

}


TEST(TestCliffWorld, Test_Get_Dynamics)
{

    CliffWorld env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);

    auto step_result = env.step(0);

    ASSERT_TRUE(step_result.mid());
    ASSERT_DOUBLE_EQ(step_result.get_extra<real_t>("prob"), 0.3333333333333333);

    auto dynamics = env.p(1, 3);
    ASSERT_EQ(dynamics.size(), static_cast<uint_t>(1));

}
