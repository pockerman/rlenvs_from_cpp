#include "gymfcpp/numpy_cpp_utils.h"
#include "gymfcpp/gymfcpp_types.h"


#include <gtest/gtest.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace{

using rlenvs_cpp::uint_t;
using rlenvs_cpp::real_t;

}

TEST(TestDisigtize, MinBound) {

    try{

        std::vector<uint_t> bins{1, 2, 3};
        auto bin = rlenvs_cpp::digitize(static_cast<uint_t>(0), bins);
        ASSERT_EQ(bin, static_cast<uint_t>(0));
    }
    catch(...)
    {
        FAIL()<<"An unknown exception occured";
    }
}

TEST(TestDisigtize, MaxBound)
{

    try{

        std::vector<uint_t> bins{1, 2, 3};
        auto bin = rlenvs_cpp::digitize(static_cast<uint_t>(4), bins);
        ASSERT_EQ(bin, static_cast<uint_t>(3));
    }
    catch(...){
        FAIL()<<"An unknown exception occured";
    }
}

TEST(TestDisigtize, InBound)
{

    try{
        std::vector<real_t> bins{1., 2., 3.};
        auto bin = rlenvs_cpp::digitize(1.5, bins);
        ASSERT_EQ(bin, static_cast<uint_t>(1));
    }
    catch(...){
        FAIL()<<"An unknown exception occured";
    }
}

TEST(TestDisigtize, InBoundLowerBound)
{

    try{
        std::vector<real_t> bins{1., 2., 3.};
        auto bin = rlenvs_cpp::digitize(1., bins);
        ASSERT_EQ(bin, static_cast<uint_t>(1));
    }
    catch(...){
        FAIL()<<"An unknown exception occured";
    }
}

TEST(TestDisigtize, OutBoundMaxBound)
{

    try{
        std::vector<real_t> bins{1., 2., 3.};
        auto bin = rlenvs_cpp::digitize(3., bins);

        // this sould also be out of bounds
        ASSERT_EQ(bin, static_cast<uint_t>(3));
    }
    catch(...){
        FAIL()<<"An unknown exception occured";
    }
}



