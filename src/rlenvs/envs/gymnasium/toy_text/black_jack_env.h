#ifndef BLACK_JACK_H
#define BLACK_JACK_H

/**
 *  BlackJack environment
 *  https://github.com/Farama-Foundation/Gymnasium/blob/main/gymnasium/envs/toy_text/blackjack.py
 *
 *
 *
 *
 */

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/time_step.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/envs/gymnasium/toy_text/toy_text_base.h"

#include <string>
#include <vector>
#include <tuple>
#include <any>
#include <unordered_map>

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium {


BETTER_ENUM(BlackJackActionsEnum, int, STICK=0, HIT=1, INVALID_ACTION=2);

///
/// \brief The CliffWorldData struct
///
struct BlackJackData
{
    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<2> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_type
    ///
    typedef DiscreteSpace<48> state_space_type;

    ///
    /// \brief state_type
    ///
    typedef std::vector<std::tuple<uint_t, uint_t, uint_t> > state_type;


    ///
    /// \brief name
    ///
    static  const std::string name;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

};

///
/// \brief The BlackJack class. Wrapper to the Blackjack
/// OpenAI-Gym environment.
///
class BlackJack final:  public ToyTextEnvBase<BlackJackData::time_step_type>
{

public:

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_type;

    ///
    /// \brief env_data_t
    ///
    typedef BlackJackData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef BlackJackData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef BlackJackData::action_type action_type;

    ///
    /// \brief state_space_t
    ///
    typedef BlackJackData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef BlackJackData::state_type state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef BlackJackData::time_step_type time_step_type;

    ///
    /// \brief action_t
    ///
    typedef uint_t action_t;

    ///
    /// \brief state_t
    ///
    typedef uint_t state_t;


    /**
     * @brief Convert the action index to a valid FrozenLakeActionsEnum
     *
     * */
    static BlackJackActionsEnum action_from_int(uint_t aidx);

    ///
    /// \brief BlackJack. Constructor.
    ///
    BlackJack(const std::string& api_base_url);

    ///
    /// \brief ~BlackJack. Destructor
    ///
    ~BlackJack()=default;

   ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) override final;

    ///
    /// \brief n_states
    ///
    uint_t n_states()const{return state_space_type::size;}

    ///
    /// \brief n_actions
    /// \return
    ///
    uint_t n_actions()const noexcept{return action_space_type::size; }

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    time_step_type step(BlackJackActionsEnum action);

    /**
     * @brief Step in the environment following the given action
     *
     * */
    time_step_type step(uint_t action);

    ///
    ///
    ///
    bool is_natural()const noexcept{return is_natural_;}

    ///
    ///
    ///
    bool is_sab()const noexcept{return is_sab_;}


protected:

    ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const override final;

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;



private:

    ///
    /// \brief Flag indicating if the environment has been
    /// initialized as natural
    ///
    bool is_natural_{false};
    bool is_sab_{false};

    void make_natural()noexcept{is_natural_ = true;}
    void make_sab()noexcept{is_sab_ = true;}

};

}
}
}
#endif // BLACK_JACK_H
