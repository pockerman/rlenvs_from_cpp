#ifndef CONTINUOUS_SPACE_H
#define CONTINUOUS_SPACE_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"

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

template<>
struct ContinuousSpace<1>
{
	
	///
    /// \brief item_t
    ///
    typedef real_t item_t;

    ///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = 1;

};


}

#endif // CONTINUOUS_SPACE_H
