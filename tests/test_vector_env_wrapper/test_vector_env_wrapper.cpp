#include "gymfcpp/state_aggregation_cart_pole_env.h"
#include "gymfcpp/time_step.h"
#include "gymfcpp/time_step_type.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/render_mode_enum.h"

#include <gtest/gtest.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace{

using gymfcpp::uint_t;
using gymfcpp::real_t;

}


TEST(TestStateAggregationCartPole, TestConstructor) {

    try{

        Py_Initialize();
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");
        gymfcpp::StateAggregationCartPole env("v0", main_namespace, 10);
    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL();
    }

}


TEST(TestStateAggregationCartPole, TestMake)
{

    try{

        Py_Initialize();
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::StateAggregationCartPole env("v0", main_namespace, 10);
        env.make();

        ASSERT_EQ(env.n_actions(), static_cast<uint_t>(2));

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not make the environment";
    }
}


TEST(TestStateAggregationCartPole, TestReset)
{

    try{

        Py_Initialize();
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::StateAggregationCartPole env("v0", main_namespace, 10);
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

TEST(TestStateAggregationCartPole, TestStep)
{

    try{

        Py_Initialize();
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::StateAggregationCartPole env("v0", main_namespace, 10);
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


TEST(TestStateAggregationCartPole, TestRender)
{

    try{

        Py_Initialize();
        boost::python::numpy::initialize();
        auto main_module = boost::python::import("__main__");
        auto main_namespace = main_module.attr("__dict__");

        gymfcpp::StateAggregationCartPole env("v0", main_namespace, 10);
        env.make();
        env.reset();

        env.render(gymfcpp::RenderModeType::human);

    }
    catch(const boost::python::error_already_set&)
    {
        PyErr_Print();
        FAIL()<<"Error could not step in the environment";
    }
}



