#include "gymfcpp/black_jack_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::gymfcpp::BlackJack;

}


TEST(TestBlackJack, TestConstructor) {

    try{

        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        BlackJack env("v0", gym_namespace, false);
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }

}

TEST(TestBlackJack, Test_Make_NO_NATURAL)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        BlackJack env("v0", gym_namespace, false);
        env.make();
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(2));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}

TEST(TestBlackJack, Test_Make_NATURAL)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        BlackJack env("v0", gym_namespace, false);
        env.make(true);
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(2));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}


TEST(TestBlackJack, Test_N_Actions_Not_Created_Assert_Is_Thrown)
{

    try{

        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        BlackJack env("v0", gym_namespace, false);

        ASSERT_DEATH(env.n_actions(), "Environment has not been created");

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}


TEST(TestBlackJack, Test_Reset)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        BlackJack env("v0", gym_namespace, false);
        env.make();

        auto state = env.reset();
        ASSERT_TRUE(state.first());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not reset the environment";
    }
}

TEST(TestBlackJack, Test_Step)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        BlackJack env("v0", gym_namespace, false);
        env.make();

        auto step_result = env.step(0);

        // step may finish the game so not sure
        // if the time step will MID
        //ASSERT_TRUE(step_result.mid());
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestBlackJack, Test_Observation_Space)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        BlackJack env("v0", gym_namespace, false);
        env.make();

        auto obs_space = env.observation_space();
        ASSERT_EQ(obs_space.size(), static_cast<uint_t>(3))<<"Invalid observation space size";
        ASSERT_EQ(obs_space[0], static_cast<uint_t>(32))<<"Invalid observation space first dimension size";
        ASSERT_EQ(obs_space[1], static_cast<uint_t>(11))<<"Invalid observation space second size";
        ASSERT_EQ(obs_space[2], static_cast<uint_t>(2))<<"Invalid observation space third size";
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

