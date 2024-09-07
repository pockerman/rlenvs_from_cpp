#include "rlenvs/envs/gymnasium/toy_text/black_jack_env.h"
#include "rlenvs/rlenvs_types_v2.h"


#include <gtest/gtest.h>
#include <unordered_map>
#include <string>

namespace{

const std::string SERVER_URL = "http://0.0.0.0:8001/api";
using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::envs::gymnasium::BlackJack;

}

/*TEST(TestBlackJack, TestConstructor) {

         BlackJack env(SERVER_URL);
         ASSERT_EQ(env.n_actions(), static_cast<uint_t>(2));

}

TEST(TestBlackJack, Test_Make_NO_NATURAL){

    BlackJack env(SERVER_URL);
    std::unordered_map<std::string, std::any> options;
    options["natural"] = false;
    env.make("v1", options);
}

TEST(TestBlackJack, Test_Make_NATURAL){
    BlackJack env(SERVER_URL);
    std::unordered_map<std::string, std::any> options;
    options["natural"] = true;
    env.make("v1", options);
}

TEST(TestBlackJack, Test_Reset){
    BlackJack env(SERVER_URL);
    std::unordered_map<std::string, std::any> options;
    options["natural"] = true;
    env.make("v1", options);

    auto state = env.reset();
    ASSERT_TRUE(state.first());
}*/

TEST(TestBlackJack, Test_Step)
{

    BlackJack env(SERVER_URL);
    std::unordered_map<std::string, std::any> options;
    options["natural"] = true;
    env.make("v1", options);

    auto state = env.reset();

    //ASSERT_TRUE(state.first());

    auto step_result = env.step(rlenvs_cpp::envs::gymnasium::BlackJackActionsEnum::STICK);

    // step may finish the game so not sure
    // if the time step will MID
    //ASSERT_TRUE(step_result.mid());

}

