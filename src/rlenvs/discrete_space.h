#ifndef DISCRETE_SPACE_H
#define DISCRETE_SPACE_H

#include "rlenvs/rlenvs_types_v2.h"

#include <random>

namespace rlenvs_cpp {

template<uint_t SpaceSize>
struct DiscreteSpace
{

    ///
    /// \brief item_t
    ///
    typedef uint_t space_item_type;


    ///
    /// \brief size
    ///
    static constexpr uint_t size = SpaceSize;

    ///
    /// \brief sample
    /// \return
    ///
    static space_item_type sample();

    ///
    /// \brief sample
    /// \param seed
    /// \return
    ///
    static space_item_type sample(uint_t seed);
};

template<uint_t SpaceSize>
typename DiscreteSpace<SpaceSize>::space_item_type
DiscreteSpace<SpaceSize>::sample(){

    std::uniform_int_distribution<> dist(0, SpaceSize - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    return dist(gen);
}

template<uint_t SpaceSize>
typename DiscreteSpace<SpaceSize>::space_item_type
DiscreteSpace<SpaceSize>::sample(uint_t seed){

    std::uniform_int_distribution<> dist(0, SpaceSize - 1);
    std::mt19937 gen(seed);
    return dist(gen);
}

}

#endif // DISCRETE_SPACE_H
