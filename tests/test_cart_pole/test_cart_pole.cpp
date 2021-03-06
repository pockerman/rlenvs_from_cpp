#include "gymfcpp/cart_pole_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::gymfcpp::CartPole;

}


TEST(TestCartPole, TestConstructor) {

    try{

        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        CartPole env("v0", gym_namespace, false);
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }

}


TEST(TestCartPole, Test_Not_Created)
{

    try{

        Py_Initialize();
        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");
        CartPole env("v0", gym_namespace, false);

        ASSERT_FALSE(env.is_created);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}

TEST(TestCartPole, Test_Make)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        CartPole env("v0", gym_namespace, false);
        env.make();

        ASSERT_TRUE(env.is_created);
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(2));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}


TEST(TestCartPole, Test_Reset)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        CartPole env("v0", gym_namespace, false);
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

TEST(TestCartPole, Test_Step)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        CartPole env("v0", gym_namespace, false);
        env.make();
        env.reset();

        auto step_result = env.step(0);
        ASSERT_TRUE(step_result.mid());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestCartPole, Test_Get_Screen)
{
    try{
        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        CartPole env("v0", gym_namespace, false);
        env.make();
        env.reset();

        // access the screen
        env.get_screen();
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not get screen\n";
    }
}

TEST(TestCartPole, Test_Get_Screen_as_vector)
{
    try{
        Py_Initialize();

        auto gym_module = boost::python::import("gym");
        auto gym_namespace = gym_module.attr("__dict__");

        CartPole env("v0", gym_namespace, false);
        env.make();
        env.reset();

        // access the screen
        auto screen = env.get_screen();

        const auto& data = screen.get_as_vector();
        const auto shape = screen.shape();

        ASSERT_EQ(data.size(), static_cast<uint_t>(shape[0]));
        ASSERT_EQ(data[0].size(), static_cast<uint_t>(shape[1]));
        ASSERT_EQ(data[0][1].size(), static_cast<uint_t>(shape[2]));
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not get screen\n";
    }


}
