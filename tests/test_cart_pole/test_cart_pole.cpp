#include "rlenvs/envs/gymnasium/classic_control/cart_pole_env.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <gtest/gtest.h>


namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::envs::gymnasium::CartPole;

}


TEST(TestCartPole, TestConstructor) {

    CartPole env("/dummy-url");
	
	auto url = env.get_url();
	
	ASSERT_TRUE(url == "/dummy-url/gymnasium/cart-pole-env");

}







