#include "gymfcpp/cliff_world_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/render_mode_enum.h"
#include "gymfcpp/gymfcpp_types.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>
//#include <boost/python/numpy.hpp>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;
using rlenvs_cpp::gymfcpp::CliffWorld;

}

TEST(TestCliffWorld, TestConstructor) {

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");
        CliffWorld env("v0", gym_namespace);
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }

}

TEST(TestCliffWorld, Test_Make)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();

        ASSERT_EQ(env.n_states(), static_cast<uint_t>(48));
        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(4));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }
}


TEST(TestCliffWorld, Test_Reset)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();

        auto state = env.reset();
        ASSERT_TRUE(state.first());
        ASSERT_EQ(state.observation(), static_cast<uint_t>(36));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not reset the environment";
    }
}

TEST(TestCliffWorld, Test_Step)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();

        auto step_result = env.step(0);
        ASSERT_TRUE(step_result.mid());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestCliffWorld, Test_Step_With_Query)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();

        auto step_result = env.step(0, true);
        ASSERT_TRUE(step_result.mid());
        ASSERT_DOUBLE_EQ(step_result.get_extra<real_t>("prob"), 1);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}


TEST(TestCliffWorld, Test_Get_Dynamics)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();

        auto dynamics = env.p(1, 3);

        ASSERT_EQ(dynamics.size(), static_cast<uint_t>(1) );

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestCliffWorld, Test_Not_Done)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();
        env.reset();

        auto step_result = env.step(3);

        ASSERT_FALSE(step_result.done());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestCliffWorld, Test_Done_1)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();
        env.reset();

        auto step_result = env.step(1);

        ASSERT_FALSE(step_result.done());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}

TEST(TestCliffWorld, Test_Done_2)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();
        env.reset();

        auto step_result = env.step(0);

        ASSERT_FALSE(step_result.done());

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}


TEST(TestCliffWorld, Test_render)
{

    try{

        Py_Initialize();

        auto gym_module = boost::python::import("__main__");
        auto gym_namespace = gym_module.attr("__dict__");

        CliffWorld env("v0", gym_namespace, false);
        env.make();
        env.reset();

        env.render(rlenvs_cpp::RenderModeType::human);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}
