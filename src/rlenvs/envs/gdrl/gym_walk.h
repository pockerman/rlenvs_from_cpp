// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#ifndef GYM_WALK_H
#define GYM_WALK_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/envs/synchronized_env_mixin.h"
#include "rlenvs/extern/enum.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/extern/nlohmann/json/json.hpp"

#include <boost/noncopyable.hpp>
#include <vector>
#include <tuple>
#include <string>
#include <any>
#include <unordered_map>

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

namespace rlenvscpp{

/// Forward declaration
template<typename StateTp> class TimeStep;

namespace envs{
namespace gdrl{


BETTER_ENUM(GymWalkActionsEnum, int, WEST=0, EAST=1, INVALID_ACTION=4);


/**
 * @brief class GymWalk
 */
template<uint_t state_size>
class GymWalk final: private boost::noncopyable, protected synchronized_env_mixin
{
public:

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<2> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    /**
     * @brief The state space type
     *
     *
     */
    typedef DiscreteSpace<state_size> state_space_type;

    /**
     * @brief The state_type
     *
     */
    typedef typename state_space_type::item_t state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

    ///
    /// \brief name
    ///
    static  const std::string name;

    /**
     * @brief Convert the action index to a valid GymWalkActionsEnum
     *
     * */
    static GymWalkActionsEnum action_from_int(uint_t aidx);


    /**
     *
     * @brief Constructor
     *
     **/
    GymWalk(const std::string& api_base_url);


    /*
     * Expose functionality
     *
     */
    using synchronized_env_mixin::sync;


    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    void make(const std::string& version,
              const std::unordered_map<std::string, std::any>& options);


    ///
    /// \brief n_states. Returns the number of states
    ///
    uint_t n_states()const noexcept{ return state_space_type::size; }

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    time_step_type step(GymWalkActionsEnum action);


    /**
     * @brief Step in the environment following the given action
     *
     * */
    time_step_type step(uint_t action);

private:

    ///
    /// \brief The urls of the server
    ///
    const std::string url_;

    ///
    /// \brief The version of the environment
    ///
    std::string version_;

    ///
    /// \brief Flag indicating if the environment has been created
    ///
    bool is_created_;

    /**
     * @brief Number of states
     *
     */
    uint_t n_states_;

    ///
    /// \brief current_state
    ///
    time_step_type current_state_;

    ///
    /// \brief build the dynamics from response
    ///
    dynamics_t build_dynamics_from_response_(const http::Response& response)const;

    ///
    /// \brief Handle the reset response from the environment server
    ///
    time_step_type create_time_step_from_response_(const http::Response& response) const;
};

template<uint_t state_size>
const std::string GymWalk<state_size>::name = "GymWalk";

template<uint_t state_size>
GymWalkActionsEnum
GymWalk<state_size>::action_from_int(uint_t aidx){

    if(aidx==0)
        return GymWalkActionsEnum::WEST;

    if(aidx==1)
        return GymWalkActionsEnum::EAST;

    return GymWalkActionsEnum::INVALID_ACTION;
}

template<uint_t state_size>
GymWalk<state_size>::GymWalk(const std::string& api_base_url)
:
url_(api_base_url),
is_created_(false),
current_state_()
{}

template<uint_t state_size>
typename GymWalk<state_size>::dynamics_t
GymWalk<state_size>::build_dynamics_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;
    json j = json::parse(str_response);

    auto dynamics = j["dynamics"];
    return dynamics;
}

template<uint_t state_size>
typename GymWalk<state_size>::time_step_type
GymWalk<state_size>::create_time_step_from_response_(const http::Response& response)const{

    auto str_response = std::string(response.body.begin(), response.body.end());
    using json = nlohmann::json;

    json j = json::parse(str_response);

    auto step_type = j["time_step"]["step_type"];
    auto reward = j["time_step"]["reward"];
    auto discount = j["time_step"]["discount"];
    auto observation = j["time_step"]["observation"];
    auto info = j["time_step"]["info"];
    return GymWalk::time_step_type(time_step_type_from_int(step_type),
                                                 reward, observation, discount,
                                                 std::unordered_map<std::string, std::any>());
}

template<uint_t state_size>
void
GymWalk<state_size>::make(const std::string& version,
           const std::unordered_map<std::string, std::any>& options){

}

template<uint_t state_size>
typename GymWalk<state_size>::time_step_type
GymWalk<state_size>::step(GymWalkActionsEnum action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

 return current_state_;
}

template<uint_t state_size>
typename GymWalk<state_size>::time_step_type
GymWalk<state_size>::step(uint_t action){

#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif

    auto action_enum = GymWalk::action_from_int(action);
    return step(action_enum);
}


}
}
}

#endif // GYM_WALK_H
