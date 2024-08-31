#include "rlenvs/envs/gymnasium/toy_text/frozen_lake_env.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <gtest/gtest.h>
#include <unordered_map>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;

const std::string SERVER_URL = "http://0.0.0.0:8001/api";

}

TEST(TestFrozenLake, TestConstructor4x4) {

    rlenvs_cpp::envs::gymnasium::FrozenLake<4> env(SERVER_URL);

    ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
    ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
    ASSERT_EQ(env.map_type(), "4x4");

}

TEST(TestFrozenLake, TestConstructor8x8) {

    rlenvs_cpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);
    ASSERT_EQ(env.n_states(), static_cast<uint_t>(64));
    ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
    ASSERT_EQ(env.map_type(),"8x8");
}

TEST(TestFrozenLake, Test_Make){
    rlenvs_cpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);
}


TEST(TestFrozenLake, Test_Reset){

    rlenvs_cpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);

    auto state = env.reset();
    ASSERT_TRUE(state.first());

}

TEST(TestFrozenLake, Test_Step)
{

    rlenvs_cpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);

    auto step_result = env.step(0);
    ASSERT_TRUE(step_result.mid());

}

TEST(TestFrozenLake, Test_Step_With_Query){
    rlenvs_cpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);

    auto step_result = env.step(0);
    ASSERT_TRUE(step_result.mid());

    ASSERT_DOUBLE_EQ(step_result.get_extra<real_t>("prob"), 0.3333333333333333);
}


TEST(TestFrozenLake, Test_Get_Dynamics){

    rlenvs_cpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);

    auto step_result = env.step(0);
    ASSERT_TRUE(step_result.mid());

    ASSERT_DOUBLE_EQ(step_result.get_extra<real_t>("prob"), 0.3333333333333333);

    auto dynamics = env.p(1, 3);
    ASSERT_EQ(dynamics.size(), static_cast<uint_t>(3));
}



