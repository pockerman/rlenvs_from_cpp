
#include "gymfcpp/serial_vector_env_wrapper.h"

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

using rlenvscpp::uint_t;
using rlenvscpp::real_t;
using rlenvscpp::gymfcpp::CartPole;
using rlenvscpp::SerialVectorEnvWrapper;
using rlenvscpp::SerialVectorEnvWrapperConfig;

}


TEST(SerialVectorEnvWrapper, Constructor) {

    try{

        Py_Initialize();

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        SerialVectorEnvWrapperConfig config;
        config.env_id = "v0";

        SerialVectorEnvWrapper<CartPole> env(config, main_namespace);
        env.make();
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}


TEST(SerialVectorEnvWrapper, TestReset)
{

    try{

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        SerialVectorEnvWrapperConfig config;
        config.env_id = "v0";
        config.n_copies = 2;

        SerialVectorEnvWrapper<CartPole> env(config, main_namespace);
        env.make();

        auto time_step = env.reset();

        ASSERT_EQ(time_step.size(), static_cast<uint_t>(2));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not make the environment";
    }
}


TEST(TestStateAggregationCartPole, TestStep)
{

    try{

        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        SerialVectorEnvWrapperConfig config;
        config.env_id = "v0";
        config.n_copies = 2;

        SerialVectorEnvWrapper<CartPole> env(config, main_namespace);
        env.make();

        env.reset();
        std::vector<CartPole::action_type> actions(env.n_copies(), 0);

        auto time_step = env.step(actions);

        ASSERT_EQ(time_step.size(), static_cast<uint_t>(2));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not reset the environment";
    }
}






