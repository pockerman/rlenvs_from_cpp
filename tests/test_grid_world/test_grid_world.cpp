#include "rlenvs/envs/grid_world/grid_world_env.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/rlenvs_consts.h"


#include <gtest/gtest.h>
#include <unordered_map>
#include <string>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::envs::grid_world::GridWorldInitType;
using rlenvs_cpp::envs::grid_world::GridWorldActionType;


}

TEST(TestGridworld, TestConstructor4x4) {

        rlenvs_cpp::envs::grid_world::Gridworld<4> env;

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_FALSE(env.is_created());

        // TODO: Think how to test this
        ASSERT_TRUE(rlenvs_cpp::envs::grid_world::to_string(env.init_type()) == rlenvs_cpp::envs::grid_world::to_string(GridWorldInitType::INVALID_TYPE));
        ASSERT_EQ(env.name, "Gridworld");
        ASSERT_EQ(env.version(), rlenvs_cpp::INVALID_STR);
}

TEST(TestGridworld, TestMake) {

        rlenvs_cpp::envs::grid_world::Gridworld<4> env;

        std::unordered_map<std::string, std::any> options;
        options["mode"] = std::any(rlenvs_cpp::envs::grid_world::to_string(GridWorldInitType::STATIC));

        env.make("v0", options);

        ASSERT_TRUE(env.is_created());
        ASSERT_EQ(env.n_states(), static_cast<uint_t>(16));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));
        ASSERT_EQ(env.version(), "v0");
        ASSERT_TRUE(rlenvs_cpp::envs::grid_world::to_string(env.init_type()) == rlenvs_cpp::envs::grid_world::to_string(GridWorldInitType::STATIC));

}

TEST(TestGridworld, TestSTATICBoardCreation) {

    const auto board_size = 4;
    rlenvs_cpp::envs::grid_world::detail::board env;

    env.init_board(board_size, GridWorldInitType::STATIC);
    auto state = env.get_state();

    ASSERT_EQ(state.size(), board_size);

    // we haven't made a move so reward should be negative
    ASSERT_TRUE(env.get_reward() < 0);
}

TEST(TestGridworld, TestSTATICBoardStepUP) {

    const auto board_size = 4;
    rlenvs_cpp::envs::grid_world::detail::board env;

    env.init_board(board_size, GridWorldInitType::STATIC);
    auto state = env.step(GridWorldActionType::UP);

    ASSERT_EQ(state.size(), board_size);

    // we haven't made a move so reward should be negative
    ASSERT_TRUE(env.get_reward() < 0);
}

TEST(TestGridworld, TestSTATICBoardStepDOWN) {

    const auto board_size = 4;
    rlenvs_cpp::envs::grid_world::detail::board env;

    env.init_board(board_size, GridWorldInitType::STATIC);
    auto state = env.step(GridWorldActionType::DOWN);

    ASSERT_EQ(state.size(), board_size);

    // we haven't made a move so reward should be negative
    ASSERT_TRUE(env.get_reward() > 0);
}

TEST(TestGridworld, TestSTATICBoardStepRIGHT) {

    const auto board_size = 4;
    rlenvs_cpp::envs::grid_world::detail::board env;

    env.init_board(board_size, GridWorldInitType::STATIC);
    auto state = env.step(GridWorldActionType::RIGHT);

    ASSERT_EQ(state.size(), board_size);

    // we haven't made a move so reward should be negative
    ASSERT_TRUE(env.get_reward() > 0);
}

TEST(TestGridworld, TestSTATICBoardStepLEFT) {

    const auto board_size = 4;
    rlenvs_cpp::envs::grid_world::detail::board env;

    env.init_board(board_size, GridWorldInitType::STATIC);
    auto state = env.step(GridWorldActionType::LEFT);

    ASSERT_EQ(state.size(), board_size);

    // we haven't made a move so reward should be negative
    ASSERT_TRUE(env.get_reward() > 0);
}
