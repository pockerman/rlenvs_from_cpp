#include "rlenvs/envs/gymnasium/classic_control/cart_pole_env.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <gtest/gtest.h>

#include <unordered_map>

namespace{

const std::string SERVER_URL = "http://0.0.0.0:8001/api";
using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::envs::gymnasium::CartPole;

}


TEST(TestCartPole, TestConstructor) {

    CartPole env("/dummy-url");
	
	auto url = env.get_url();
	ASSERT_TRUE(url == "/dummy-url/gymnasium/cart-pole-env");

}


TEST(TestCartPole, Test_Make)
{
    CartPole env(SERVER_URL);

    std::unordered_map<std::string, std::any> options;
    env.make("v1", options);

    auto state = env.reset();
    ASSERT_TRUE(state.first());
}







