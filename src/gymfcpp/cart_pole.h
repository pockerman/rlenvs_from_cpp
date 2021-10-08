#ifndef CART_POLE_H
#define CART_POLE_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/data_wrapper.h"


#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace gymfcpp{

/// Forward declaration
template<typename StateTp> class TimeStep;

///
/// \brief The CartPole class Interface for CartPole environment
///
class CartPole: private boost::noncopyable
{

public:

    ///
    /// \brief action_t
    ///
    typedef uint_t action_t;

    ///
    /// \brief state_t
    ///
    typedef std::vector<real_t> state_t;

    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief py_env_name
    ///
    static std::string py_env_name;

    ///
    /// \brief py_step_result_name
    ///
    static std::string py_step_result_name;

    ///
    /// \brief CartPole. Constructor
    ///
    CartPole(const std::string& version, obj_t gym_namespace, bool do_create=true);

    ///
    /// \brief make
    /// \param is_slipery
    ///
    void make();

    ///
    /// \brief n_actions
    /// \return
    ///
    uint_t n_actions()const;

    ///
    /// \brief version
    ///
    std::string_view version()const noexcept{return data_.v;}

    ///
    /// \brief is_created
    /// \return
    ///
    bool is_created()const noexcept{return data_.is_created;}

    ///
    /// \brief reset
    /// \return
    ///
    TimeStep<state_t> reset();

    ///
    /// \brief step
    ///
    TimeStep<state_t> step(action_t action);

private:


    ///
    /// \brief data_
    ///
    detail::data_wrapper data_;

    /// cached data

    ///
    /// \brief number of actions
    ///
    mutable uint_t n_actions_;

};
}

#endif // CART_POLE_H
