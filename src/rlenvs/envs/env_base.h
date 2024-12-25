// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#ifndef ENV_BASE_H
#define ENV_BASE_H

#include "rlenvs/rlenvs_consts.h"
#include "rlenvs/envs/synchronized_env_mixin.h"
#include <boost/noncopyable.hpp>

#include <unordered_map>
#include <any>
#include <string>
#include <memory>

namespace rlenvscpp{
namespace envs{


///
/// \ brief Base class for environments.
///
template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
class EnvBase: private boost::noncopyable, public ActionSpaceType, public synchronized_env_mixin
{
public:

	///
	/// \brief The time step type we return every time a step in the
	/// environment is performed
	///
    typedef TimeStepType time_step_type;
	
	///
	/// \brief The type describing the state space for the environment
	///
	typedef StateSpaceType state_space_type;
	
	///
	/// \brief The type of the action space for the environment
	///
	typedef ActionSpaceType action_space_type;

    ///
	/// \brief The type of the action to be undertaken in the environment
	///
    typedef typename action_space_type action_type;

    ///
	/// \brief dctor
	///
    virtual ~EnvBase()=default;

    ///
	/// \brief make. Builds the environment. 
	///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) = 0;

    ///
	/// \brief close the environment
	///
    virtual void close()=0;

    /// 
	/// \brief Reset the environment
	///
    virtual time_step_type reset(uint_t seed,
                                 const std::unordered_map<std::string, std::any>& options)=0;
								 
	 ///
	 /// \brief Reset the environment always using the same seed
	 ///
    time_step_type reset(const std::unordered_map<std::string, std::any>& options = std::unordered_map<std::string, std::any>()){
        return reset(DEFAULT_ENV_SEED, options);}
		
    ///
	/// \brief step in the environment by performing the given action
    /// \param action The action to execute in the environment 
	/// \return The time step 
    virtual time_step_type step(const action_type& action)=0;
	
	///
	/// \brief Create a new copy of the environment with the given
	/// copy index
	///
	virtual std::unique_ptr<EnvBase<time_step_type, StateSpaceType ActionSpaceType>> make_copy(uint_t cidx)const=0;
	
    ///
	/// \brief is_created Returns true is make has been called successfully
	///
    bool is_created()const noexcept{return is_created_;}

    ///
	/// \brief version
	///
    std::string version()const noexcept{return version_;}

    ///
	/// \brief name of the environment
	///
    std::string env_name()const noexcept{return name_;}

    ///
	/// \brief The number of agents acting in the environment
	///
    uint_t cidx()const noexcept{return cidx_;}

protected:


    ///
	/// \brief Constructor
	///
    explicit EnvBase(const uint_t cidx=0, const std::string& name=INVALID_STR);


    ///
	/// \brief Helper function to set the version. To be called only when the make is called
	///
    void set_version_(const std::string& version )noexcept{version_ = version;}


    void invalidate_is_created_flag_()noexcept{is_created_ = false;}

    ///
	/// \brief mark the environment as created
	///
    void make_created_()noexcept{is_created_= true;}
	
	time_step_type& get_current_time_step_()noexcept{return current_state_;}
    const time_step_type& get_current_time_step_()const noexcept{return current_state_;}
	
private:

    bool is_created_;
    uint_t cidx_;
    std::string version_;
    const std::string name_;
	
	///
    /// \brief current_state
    ///
    time_step_type current_state_;
};


template<typename TimeStepType, typename StateSpaceType, typename ActionSpaceType>
EnvBase<TimeStepType, StateSpaceType, ActionSpaceType>::EnvBase(const uint_t cidx, const std::string& name)
:
ActionSpaceType(),
synchronized_env_mixin(),
is_created_(false),
cidx_(cidx),
version_(),
name_(name),
current_state_()
{}

}
}

#endif // ENV_BASE_H
