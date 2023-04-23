#ifndef VECTOR_CART_POLE_H
#define VECTOR_CART_POLE_H

#include "rlenvs/rlenvs_types.h"

#include <string>
namespace rlenvs_cpp{
namespace envs{
namespace gymnasium{
namespace vector{

class VCartPole
{
public:


    ///
    /// \brief VCartPole Constructor
    ///
    VCartPole(const std::string& version, obj_t main_namespace,
              uint_t n_copies=1,
              bool do_create=true);

    ///
    /// \brief make Create the vector environment
    ///
    void make();

private:


    ///
    /// \brief version
    /// \return
    ///
    std::string version_;

    ///
    /// \brief is_created
    ///
    bool is_created;

    ///
    /// \brief gym_namespace_
    ///
    obj_t gym_namespace;

    ///
    /// \brief env
    ///
    obj_t env;

    uint_t n_copies_;
};

}

}
}
}

#endif // VECTOR_CART_POLE_H
