#ifndef MOUNTAIN_CAR_H
#define MOUNTAIN_CAR_H

#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/envs/gymnasium/gymnasium_env_base.h"
#include "rlenvs/discrete_space.h"
#include "rlenvs/continuous_space.h"
#include "rlenvs/time_step.h"
#include "rlenvs/extern/enum.h"


#include <string>
#include <vector>
#include <tuple>
#include <any>
#include <unordered_map>

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium{

BETTER_ENUM(MountainCarActionsEnum, int, ACCELERATE_LEFT=0, 
										 DO_NOT_ACCELERATE=1, 
										 ACCELERATE_RIGHT=2, 
										 INVALID_ACTION=3);

///
/// \brief The MountainCarData struct. Wrapper for the environment data
///
struct MountainCarData
{
    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef DiscreteSpace<3> action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_t
    ///
    typedef ContinuousSpace<3> state_space_type;

    ///
    /// \brief state_t
    ///
    typedef state_space_type::item_t state_type;

    

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;


};

///
/// \brief The MountainCar class
///
class MountainCar final:  public GymnasiumEnvBase<MountainCarData::time_step_type>
{

public:
	
	///
    /// \brief name
    ///
    static  const std::string name;

    ///
    /// \brief env_data_t
    ///
    typedef MountainCarData  env_data_type;

    ///
    /// \brief action_space_t. The type of the action space
    ///
    typedef MountainCarData::action_space_type action_space_type;

    ///
    /// \brief action_t
    ///
    typedef action_space_type::item_t action_type;

    ///
    /// \brief state_space_t
    ///
    typedef MountainCarData::state_space_type state_space_type;

    ///
    /// \brief state_t
    ///
    typedef state_space_type::item_t state_type;

    ///
    /// \brief time_step_t. The type of the time step
    ///
    typedef TimeStep<state_type> time_step_type;

    ///
    /// \brief MountainCar. Constructor. Creates an environment.
    /// \param version The version of the environment
    /// \param gym_namespace The boost::python open-ai gym namespace
    /// \param do_create If true it calls make
    ///
    MountainCar(const std::string& api_base_url);

    ///
    /// \brief ~MountainCar. Destructor.
    ///
    ~MountainCar()=default;

    ///
    /// \brief make. Build the environment
    ///
    void make(const std::string& version,
              const std::unordered_map<std::string, std::any>& /*options*/) override final;

    ///
    /// \brief n_actions. Returns the number of actions
    ///
    uint_t n_actions()const noexcept{return action_space_type::size;}

    ///
    /// \brief step
    ///
    time_step_type step(MountainCarActionsEnum action);

    ///
    /// \brief sample Samples an action from the action space
    /// \return
    ///
    action_type sample_action_id()const noexcept{return MountainCarData::action_space_type::sample();}

    ///
    ///
    ///
    MountainCarActionsEnum sample_action()const;

     /**
     * @brief Synchronize the environment
     *
     */
    void sync(const std::unordered_map<std::string, std::any>& /*options*/=std::unordered_map<std::string, std::any>()){}


protected:

    ///
    /// \brief Handle the reset response from the environment server
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response) const override final;



};
}
}
}
#endif // MOUNTAIN_CAR_H
