#ifndef FROZEN_LAKE_H
#define FROZEN_LAKE_H

#include "gymfcpp/types.h"
#include "gymfcpp/time_step.h"

#include <boost/noncopyable.hpp>
#include <string>

namespace gymfcpp {

///
class TimeStep;

///
/// \brief The FrozenLake class
///
class FrozenLake: private boost::noncopyable
{
public:

    ///
    /// \brief state_t
    ///
    typedef uint_t state_t;

    ///
    /// \brief action_t
    ///
    typedef uint_t action_t;

    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief FrozenLake
    /// \param version
    ///
    FrozenLake(std::string version, obj_t gym_namespace, bool do_create=true, bool is_slipery=true);

    ///
    /// \brief make
    /// \param is_slipery
    ///
    void make(bool is_slipery=true);

    ///
    /// \brief n_states
    ///
    uint_t n_states()const;

    ///
    /// \brief n_actions
    /// \return
    ///
    uint_t n_actions()const;

    ///
    /// \brief version
    ///
    std::string_view version()const{return v_;}

    ///
    /// \brief is_created
    /// \return
    ///
    bool is_created()const{return is_created_;}

    ///
    /// \brief reset
    /// \return
    ///
    TimeStep reset();

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    TimeStep step(action_t action, bool query_extra=false);

private:

    ///
    /// \brief v_
    ///
    std::string v_;

    ///
    /// \brief is_created_
    ///
    bool is_created_;

    ///
    /// \brief gym_namespace_
    ///
    obj_t gym_namespace_;

    ///
    /// \brief world_ The obj populated when make is called
    ///
    obj_t world_;

    ///
    /// \brief current_state_ Keep tract what the current
    /// state of the world is
    ///
    TimeStep current_state_;
};

}

#endif // FROZEN_LAKE_H
