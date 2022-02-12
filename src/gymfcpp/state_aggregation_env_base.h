#ifndef STATE_AGGREGATION_ENV_BASE_H
#define STATE_AGGREGATION_ENV_BASE_H

#include "gymfcpp/gymfcpp_types.h"
# include "gymfcpp/env_mixin.h"
#include "gymfcpp/render_mode_enum.h"
#include "gymfcpp/gymfcpp_config.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <vector>
#include <string>

namespace gymfcpp
{

/// Forward declaration
template<typename StateTp> class TimeStep;

///
/// \brief The TiledEnviromentBase class. Base class to derive
/// enviroments that discretize the continues state into a discrete number
/// of bins.
///
template<typename EnvType, typename StateTp>
class StateAggregationEnvBase: protected EnvMixin<typename EnvType::env_data_type>
{
public:

    typedef EnvType env_type;
    typedef StateTp state_type;
    typedef TimeStep<state_type> time_step_type;
    typedef typename env_type::action_type action_type;

    ///
    /// \brief Expose useful functionality
    ///
    using EnvMixin<typename EnvType::env_data_type>::full_name;
    using EnvMixin<typename EnvType::env_data_type>::render;
    using EnvMixin<typename EnvType::env_data_type>::close;
    using EnvMixin<typename EnvType::env_data_type>::is_created;
    using EnvMixin<typename EnvType::env_data_type>::py_env_name;
    using EnvMixin<typename EnvType::env_data_type>::py_reset_result_name;
    using EnvMixin<typename EnvType::env_data_type>::py_step_result_name;
    using EnvMixin<typename EnvType::env_data_type>::py_state_name;
    using EnvMixin<typename EnvType::env_data_type>::idx;

    ///
    /// \brief Destructor
    ///
    virtual ~StateAggregationEnvBase()=default;

    ///
    /// \brief make
    ///
    virtual void make();

    ///
    /// \brief reset
    /// \return
    ///
    time_step_type reset();

    ///
    /// \brief step
    /// \param action
    /// \return
    ///
    virtual time_step_type step(const action_type& action)=0;

    ///
    /// \brief create_bins. Create the bins for the variables
    ///
    virtual void create_bins()=0;

    ///
    /// \brief get_state Transforms the original state to the aggregate state
    /// \param obs. The original continuous state
    /// \return
    ///
    virtual state_type get_state(const typename env_type::state_type& obs)const=0;

    ///
    /// \brief n_states
    /// \return
    ///
    uint_t n_states()const noexcept{return n_states_;}

protected:


    ///
    /// \brief Constructor
    ///
    StateAggregationEnvBase(const std::string version, obj_t main_namespace, uint_t n_states);

    ///
    /// \brief env_ The actual environment
    ///
    env_type env_;

    ///
    /// \brief n_states_
    ///
    uint_t n_states_;
};

template<typename EnvType, typename StateTp>
StateAggregationEnvBase<EnvType, StateTp>::StateAggregationEnvBase(const std::string version, obj_t main_namespace, uint_t n_states)
    :
    EnvMixin<typename EnvType::env_data_type>(version,  main_namespace),
    env_(version, main_namespace),
    n_states_(n_states)
{
#ifdef GYMFCPP_DEBUG
 assert(n_states_ != 0 && "Number of states cannot be zero");
#endif


}

template<typename EnvType, typename StateTp>
void
StateAggregationEnvBase<EnvType, StateTp>::make(){

    // create the bins
    create_bins();

    // make the environment
    env_.make();

    py_env_name = env_.py_env_name;

    py_reset_result_name = env_.py_reset_result_name;
    py_step_result_name = env_.py_step_result_name;
    py_state_name = env_.py_state_name;

    // make the flag true
    is_created = true;

}


}

#endif // TILED_ENV_BASE_H
