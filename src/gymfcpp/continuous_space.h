#ifndef CONTINUOUS_SPACE_H
#define CONTINUOUS_SPACE_H

#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/gymfcpp_types.h"

#include <vector>

namespace rlenvs_cpp
{

///
/// \brief The ContinuousSpace class.
///
template<uint_t SpaceSize>
struct ContinuousSpace
{
    ///
    /// \brief item_t
    ///
    typedef std::vector<real_t> item_t;

    ///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = SpaceSize;

};


}

#endif // CONTINUOUS_SPACE_H
