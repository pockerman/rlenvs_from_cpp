#include "rlenvs/envs/gymnasium/classic_control/cart_pole_env.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <gtest/gtest.h>

#include <unordered_map>

namespace{

const std::string SERVER_URL = "http://0.0.0.0:8001/api";
using rlenvscpp::uint_t;
using rlenvscpp::real_t;
using rlenvscpp::envs::gymnasium::CartPole;

}


TEST(TestCartPole, TestConstructor) {

    CartPole env("/dummy-url");
	auto url = env.get_url();
	ASSERT_TRUE(url == "/dummy-url/gymnasium/cart-pole-env");

}


TEST(TestCartPole, Test_Make)
{
    CartPole env(SERVER_URL);
    auto copy_env_ptr = env.make_copy(static_cast<uint_t>(1));
    ASSERT_TRUE(copy_env_ptr != nullptr);
}







