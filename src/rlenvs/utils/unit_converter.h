#ifndef UNIT_CONVERTER_H
#define UNIT_CONVERTER_H

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/rlenvs_consts.h"

namespace rlenvscpp{
namespace utils{
namespace unit_converter{
	
	///
    /// \brief Convert RPM to angular speed in m/s
    ///
    real_t rpm_to_angular_speed(uint_t rpm){return (2.0*rlenvscpp::consts::maths::PI * rpm) / 60.0;}

    ///
    /// \brief Convert RPM to angular speed in m/s
    ///
    real_t angular_speed_to_linear_speed(real_t r, real_t w){return r * w;}

    ///
    /// \brief Convert radians to degrees
    ///
    real_t rad_to_degrees(real_t rad){return (rad * 180.0) / rlenvscpp::consts::maths::PI;}

    ///
    /// \brief Convert degrees to radians
    ///
    real_t degrees_to_rad(real_t degs){return (degs * rlenvscpp::consts::maths::PI) / 180.0;}

}	
}
}


#endif