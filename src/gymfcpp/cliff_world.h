#ifndef CLIFF_WORLD_H
#define CLIFF_WORLD_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step.h"

#include "boost/noncopyable.hpp"
#include <string>


namespace gymfcpp {


///
/// \brief The CliffWorld class
///
class CliffWorld: private boost::noncopyable
{

public:

    ///
    /// \brief state_t. The type of the state
    ///
    typedef uint_t state_t;

    ///
    /// \brief action_t. The type of the action
    ///
    typedef uint_t action_t;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief name. The name of the environment
    ///
    static  std::string name;

    ///
    /// \brief py_env_name. The name of the environment created in
    /// the Python interpreter
    ///
    static std::string py_env_name;

    ///
    /// \brief py_obs_name. The name of the observation in the
    /// Python interpreter
    ///
    static std::string py_obs_name;

    ///
    /// \brief py_step_result_name
    ///
    static std::string py_step_result_name;

    ///
    /// \brief py_dynamics_name
    ///
    static std::string py_dynamics_name;

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief FrozenLake
    /// \param version
    ///
    CliffWorld(std::string version, obj_t gym_namespace, bool do_create=true);

    ///
    /// \brief ~CliffWorld. Destructor
    ///
    ~CliffWorld();

    ///
    /// \brief make
    ///
    void make();

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
    void render(std::string mode="human");

    ///
    /// \brief close. Close down the python environment
    ///
    void close();

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

    ///
    /// \brief cache_n_actions_
    ///
    mutable uint_t cached_n_actions_;

    ///
    /// \brief cached_n_states_
    ///
    mutable uint_t cached_n_states_;
};

}

#endif // CLIFF_WORLD_H
