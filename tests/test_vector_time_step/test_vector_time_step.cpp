
#include "gymfcpp/vector_time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/torch_state_adaptor.h"
#include "gymfcpp/gymfcpp_config.h"

#ifdef USE_PYTORCH
#include "gymfcpp/torch_state_adaptor.h"
#endif

#include "gymfcpp/cart_pole_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/render_mode_enum.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>

#include <chrono>
#include <thread>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::gymfcpp::CartPole;
using rlenvs_cpp::VectorTimeStep;
using rlenvs_cpp::torch_utils::TorchStateAdaptor;
using rlenvs_cpp::TimeStep;
using rlenvs_cpp::TimeStepTp;
}


TEST(TestVectorTimeStep, Constructor) {


        VectorTimeStep<std::vector<real_t>> vec_step;

        ASSERT_TRUE(vec_step.empty());
        ASSERT_EQ(vec_step.size(), static_cast<uint_t>(0));
}


TEST(TestVectorTimeStep, AddTimeStep)
{

    VectorTimeStep<std::vector<real_t>> vec_step;

    ASSERT_TRUE(vec_step.empty());
    ASSERT_EQ(vec_step.size(), static_cast<uint_t>(0));

    TimeStep<std::vector<real_t>> step(TimeStepTp::FIRST, 1.0, std::vector<real_t>(2, 2.0));

    vec_step.add_time_step(step);
    ASSERT_EQ(vec_step.size(), static_cast<uint_t>(1));

}


TEST(TestVectorTimeStep, StackState)
{

    VectorTimeStep<std::vector<real_t>> vec_step;

    ASSERT_TRUE(vec_step.empty());
    ASSERT_EQ(vec_step.size(), static_cast<uint_t>(0));

    TimeStep<std::vector<real_t>> step1(TimeStepTp::FIRST, 1.0, std::vector<real_t>(2, 2.0));

    vec_step.add_time_step(step1);

    TimeStep<std::vector<real_t>> step2(TimeStepTp::FIRST, 2.0, std::vector<real_t>(2, 3.0));
    vec_step.add_time_step(step2);

    ASSERT_EQ(vec_step.size(), static_cast<uint_t>(2));

    auto tensor = vec_step.stack_states<TorchStateAdaptor>();

    ASSERT_EQ(tensor.size(0), static_cast<long int>(2));
    ASSERT_EQ(tensor.size(1), static_cast<long int>(2));
}


TEST(TestVectorTimeStep, StackReward)
{

    VectorTimeStep<std::vector<real_t>> vec_step;

    ASSERT_TRUE(vec_step.empty());
    ASSERT_EQ(vec_step.size(), static_cast<uint_t>(0));

    TimeStep<std::vector<real_t>> step1(TimeStepTp::FIRST, 1.0, std::vector<real_t>(2, 2.0));

    vec_step.add_time_step(step1);

    TimeStep<std::vector<real_t>> step2(TimeStepTp::FIRST, 2.0, std::vector<real_t>(2, 3.0));
    vec_step.add_time_step(step2);

    ASSERT_EQ(vec_step.size(), static_cast<uint_t>(2));

    auto tensor = vec_step.stack_rewards<TorchStateAdaptor>();

    ASSERT_EQ(tensor.size(0), static_cast<long int>(2));
    //ASSERT_EQ(tensor.size(1), static_cast<long int>(2));
}






