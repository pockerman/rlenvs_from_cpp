#ifndef DISCRETE_SPACE_H
#define DISCRETE_SPACE_H

#include "gymfcpp/gymfcpp_types.h"

namespace gymfcpp {

template<uint_t SpaceSize>
struct DiscreteSpace
{

    ///
    /// \brief item_t
    ///
    typedef uint_t item_t;


    ///
    /// \brief size
    ///
    static constexpr uint_t size = SpaceSize;
};

}

#endif // DISCRETE_SPACE_H
