#ifndef VECTOR_ENV_WRAPPER_H
#define VECTOR_ENV_WRAPPER_H

/**
  * Multithreaded vector environment wrapper
  *
  *
  */

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/vector_time_step.h"

#include "boost/noncopyable.hpp"
#include <thread>
#include <string>

namespace rlenvs
{
// still we may want to use some utilities
using namespace gymfcpp;


template<typename Env, typename StateAdaptor=void>
class VectorEnvWrapper: private boost::noncopyable
{
public:

    ///
    /// \brief VectorEnvWrapper
    /// \param n_copies
    /// \param version
    ///
    VectorEnvWrapper(uint_t n_copies, std::string version);

    ///
    /// \brief make
    ///
    void make();

private:




};


}

#endif // VECTOR_ENV_WRAPPER_H
