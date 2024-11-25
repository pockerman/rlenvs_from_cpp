#ifndef TAXI_H
#define TAXI_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/toy_text/toy_text_base.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/continuous_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/extern/enum.h"

#include <map>
#include <string>
#include <any>
#include <unordered_map>


namespace rlenvs_cpp{
namespace envs {
namespace gymnasium{

BETTER_ENUM(TaxiActionsEnum, uint_t, DOWN=0, UP=1, RIGHT=2,
            LEFT=3, PICK_PASSENGER=4, DROP_PASSENGER=5, INVALID_ACTION=6);

///
/// \brief The TaxiData struct. Wrapper for the environment data
///
struct TaxiData
{
    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<6> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_t
    ///
    typedef ContinuousSpace<4> state_space_type;

    ///
    /// \brief state_t
    ///
    typedef uint_t state_type;

    

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

};


///
/// \brief The Taxi class
///
class Taxi final: public  ToyTextEnvBase<TaxiData::time_step_type>
{
public:
	
	///
    /// \brief name
    ///
    static  const std::string name;
	
    ///
    /// \brief env_data_t
    ///
    typedef TaxiData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef TaxiData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef TaxiData::action_type action_type;

    ///
    /// \brief state_space_t
    ///
    typedef TaxiData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef TaxiData::state_type state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TaxiData::time_step_type time_step_type;

    ///
    /// \brief Taxi
    ///
    Taxi(const std::string& api_base_url);


    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    ~Taxi()=default;

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& /*options*/) override final;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief Number of states. Hardcoded from here:
    /// https://github.com/Farama-Foundation/Gymnasium/blob/6baf8708bfb08e37ce3027b529193169eaa230fd/gymnasium/envs/toy_text/taxi.py#L165C9-L165C19
    ///
    uint_t n_states()const noexcept{return 500;}

    ///
    /// \brief step
    ///
    time_step_type step(TaxiActionsEnum action);

    ///
    /// \brief sample
    /// \return
    ///
    action_type sample_action()const noexcept{return action_space_type::sample();}

protected:

     ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const override final;

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;



};

}
}
}
#endif // TAXI_H
