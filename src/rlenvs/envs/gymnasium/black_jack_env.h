#ifndef BLACK_JACK_H
#define BLACK_JACK_H

#include "rlenvs/rlenvs_types.h"
#include "rlenvs/time_step.h"

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>
#include <tuple>

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium {

///
/// \brief The BlackJack class. Wrapper to the Blackjack
/// OpenAI-Gym environment.
///
class BlackJack: private boost::noncopyable
{

public:

    ///
    /// \brief name
    ///
    static  std::string name;

    ///
    /// \brief py_env_name
    ///
    static std::string py_env_name;

    ///
    /// \brief action_t
    ///
    typedef uint_t action_t;

    ///
    /// \brief state_t
    ///
    typedef uint_t state_t;

    ///
    /// \brief time_step_t
    ///
    typedef TimeStep<state_t> time_step_t;

    ///
    /// \brief BlackJack. Constructor. Construct an environment
    /// \param version: The version of the environemnt
    /// \param do_create: If set to true make() is called
    /// \param natural:
    ///
    BlackJack(std::string version, obj_t gym_namespace, bool do_create=true, bool natural=false);

    ///
    /// \brief ~BlackJack. Destructor
    ///
    ~BlackJack();

    ///
    /// \brief make
    ///
    void make(bool natural=false);

    ///
    /// \brief n_actions
    /// \return
    ///
    uint_t n_actions()const;

    ///
    /// \brief observation_space
    /// \return
    ///
    std::vector<uint_t> observation_space()const;

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
    time_step_t step(action_t action);

    ///
    /// \brief render. Render the environment
    ///
    void render(std::string mode);

    ///
    /// \brief close
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
    /// \brief cached_obs_space_
    ///
    mutable std::vector<uint_t> cached_obs_space_;

};

}
}
}
#endif // BLACK_JACK_H
