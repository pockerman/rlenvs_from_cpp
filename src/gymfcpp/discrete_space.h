#ifndef DISCRETE_SPACE_H
#define DISCRETE_SPACE_H

#include "gymfcpp/gymfcpp_types.h"

#include <random>

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

    ///
    /// \brief sample
    /// \return
    ///
    static item_t sample();
};

template<uint_t SpaceSize>
typename DiscreteSpace<SpaceSize>::item_t
DiscreteSpace<SpaceSize>::sample(){

    std::uniform_int_distribution<> dist(0, SpaceSize - 1);

    std::random_device rd;
    std::mt19937 gen(rd());

    return dist(gen);
}

}

#endif // DISCRETE_SPACE_H
