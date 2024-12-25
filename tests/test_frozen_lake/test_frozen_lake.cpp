#include "rlenvs/envs/gymnasium/toy_text/frozen_lake_env.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <any>

namespace{

using rlenvscpp::uint_t;
using rlenvscpp::real_t;

const std::string SERVER_URL = "http://0.0.0.0:8001/api";

}

TEST(TestFrozenLake, TestConstructor4x4) {

    rlenvscpp::envs::gymnasium::FrozenLake<4> env(SERVER_URL);

    ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
    ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
    ASSERT_EQ(env.map_type(), "4x4");

}

TEST(TestFrozenLake, TestConstructor8x8) {

    rlenvscpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);
    ASSERT_EQ(env.n_states(), static_cast<uint_t>(64));
    ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
    ASSERT_EQ(env.map_type(),"8x8");
}

TEST(TestFrozenLake, Test_Make){
    rlenvscpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);
}


TEST(TestFrozenLake, Test_Reset){

    rlenvscpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);

    auto state = env.reset();
    ASSERT_TRUE(state.first());

}

TEST(TestFrozenLake, Test_Step)
{

    rlenvscpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);
    env.reset();

    auto step_result = env.step(0);
    ASSERT_TRUE(step_result.mid());

}


TEST(TestFrozenLake, Test_Get_Dynamics){

    rlenvscpp::envs::gymnasium::FrozenLake<8> env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);
    env.reset();

    auto step_result = env.step(0);
    ASSERT_TRUE(step_result.mid());

    auto dynamics = env.p(1, 3);
    ASSERT_EQ(dynamics.size(), static_cast<uint_t>(3));
}



