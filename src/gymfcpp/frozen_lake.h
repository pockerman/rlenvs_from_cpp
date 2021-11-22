#ifndef FROZEN_LAKE_H
#define FROZEN_LAKE_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace gymfcpp {

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
    /// \brief time_step_t
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief py_env_name
    ///
    static std::string py_env_name;

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief FrozenLake. Constructor
    ///
    FrozenLake(const std::string& version, obj_t gym_namespace, bool do_create=true, bool is_slipery=true);

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    void make(bool is_slipery=true);

    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const;

    ///
    /// \brief version. The version of the environment
    ///
    std::string_view version()const noexcept{return v_;}

    ///
    /// \brief is_created
    ///
    bool is_created()const noexcept{return is_created_;}

    ///
    /// \brief reset
    /// \return
    ///
    time_step_t reset();

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    time_step_t step(action_t action, bool query_extra=false);

    ///
    /// \brief P
    /// \param sidx
    /// \param aidx
    ///
    dynamics_t p(uint_t sidx, uint_t aidx)const;

    ///
    /// \brief render. Render the environment
    ///
    void render();

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
    time_step_t current_state_;

};

}

#endif // FROZEN_LAKE_H
