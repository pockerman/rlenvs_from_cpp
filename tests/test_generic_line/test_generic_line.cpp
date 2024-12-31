#include "rlenvs/utils/geometry/geom_point.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/utils/geometry/generic_line.h"

#include <gtest/gtest.h>


namespace{

using rlenvscpp::uint_t;
using rlenvscpp::real_t;
using rlenvscpp::utils::geom::GeomPoint;
using rlenvscpp::utils::geom::GenericLine;

}

TEST(TestGenericLine, slope1) {

    GeomPoint<2> p0({0.0, 0.0});
	GeomPoint<2> p1({1.0, 0.0});

	GenericLine<2> line(p0, p1);
	auto slope = line.slope();

    EXPECT_DOUBLE_EQ(slope, 0.0);
    
}

TEST(TestGenericLine, slope2) {
	
	// assume the line y = -3/4 x + 6/4
	// x = 0 => y = 6/4
	// x = 4 => y = - 6/4

    GeomPoint<2> p0({0.0, 6.0/4.0});
	GeomPoint<2> p1({4.0, -6.0 /4.0});
	GenericLine<2> line(p0, p1);
	auto slope = line.slope();

    EXPECT_DOUBLE_EQ(slope, -3.0/4.0);
    
}

TEST(TestGenericLine, factor1) {

   // assume the line y = -3/4 x + 6/4
	// x = 0 => y = 6/4
	// x = 4 => y = - 6/4

    GeomPoint<2> p0({0.0, 6.0/4.0});
	GeomPoint<2> p1({4.0, -6.0 / 4.0});
	GenericLine<2> line(p0, p1);
	auto factor = line.factor();

    EXPECT_DOUBLE_EQ(factor, 6.0/4.0);
    
}

TEST(TestGenericLine, factor2) {

    GeomPoint<2> p0({0.0, 0.0});
	GeomPoint<2> p1({1.0, 0.0});
	GenericLine<2> line(p0, p1);
	auto factor = line.factor();

    EXPECT_DOUBLE_EQ(factor, 0.0);
    
}

TEST(TestGenericLine, distance1) {

    GeomPoint<2> p0({0.0, 0.0});
	GeomPoint<2> p1({1.0, 0.0});
	GenericLine<2> line(p0, p1);
	
	GeomPoint<2> p({0.5, 0.5});
	auto dist = line.distance(p);

    EXPECT_DOUBLE_EQ(dist, 0.5);
    
}


TEST(TestGenericLine, distance2) {
	
	// assume the line y = -3/4 x + 6/4
	// x = 0 => y = 6/4
	// x = 4 => y = - 6/4

    GeomPoint<2> p0({0.0, 6.0/4.0});
	GeomPoint<2> p1({4.0, -6.0 / 4.0});

	GenericLine<2> line(p0, p1);
	
	GeomPoint<2> p({0.0, 0.0});
	auto dist = line.distance(p);

    EXPECT_DOUBLE_EQ(dist, 6.0 / 5.0);
    
}













