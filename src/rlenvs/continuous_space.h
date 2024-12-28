#ifndef CONTINUOUS_SPACE_H
#define CONTINUOUS_SPACE_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <vector>

namespace rlenvscpp
{
	
template<uint_t SpaceSize, typename ItemType>
struct ContinuousSpace;

///
/// \brief The ContinuousSpace class.
///
template<uint_t SpaceSize, std::vector<real_t> >
struct ContinuousSpace
{
    ///
    /// \brief item_t
    ///
    typedef std::vector<real_t> space_item_type;

    ///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = SpaceSize;

};

template<>
struct ContinuousSpace<1, Null>
{
	
	///
    /// \brief item_t
    ///
    typedef real_t space_item_type;

    ///
    /// \brief size. The number of members in the space
    ///
    static constexpr uint_t size = 1;

};

template<uint_t SpaceSize>
using STDVectorContinuousSpace = ContinuousSpace<SpaceSize, std::vector<real_t>>;

using ScalarContinousSpace = ContinuousSpace<1, Null>;


}

#endif // CONTINUOUS_SPACE_H
