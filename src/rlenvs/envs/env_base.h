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

namespace rlenvscpp{
namespace envs{


/**
 * @brief Base class for environments
 */
template<typename TimeStepType, typename ActionType>
class EnvBase: private boost::noncopyable
{
public:

    typedef TimeStepType time_step_type;

    /*
     * @brief The type of the action to be undertaken in the environment
     *
     */
    typedef ActionType action_type;



    /**
     * @brief default environment seed for reset
     */
    static const uint_t DEFAULT_ENV_SEED = 42;

    /**
     * @brief dctor
     */
    virtual ~EnvBase()=default;


    /**
      * \brief make. Builds the environment. Optionally we can choose if the
      * environment will be slippery
      */
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) = 0;

     /**
     * @brief Synchronize the environment
     *
     */
    virtual void sync(const std::unordered_map<std::string, std::any>& /*options*/){}

    /**
     * @brief close the environment
     */
    virtual void close()=0;

    /**
      * @brief Reset the environment
      */
    virtual time_step_type reset(uint_t seed,
                                 const std::unordered_map<std::string, std::any>& options)=0;


    /**
      * \brief step
      * \param action
      * \return
      */
    virtual time_step_type step(const action_type& action)=0;

    /**
      * @brief is_created Returns true is make has been called successfully
      */
    bool is_created()const noexcept{return is_created_;}


    /**
      * @brief Returns the url the environment is using
      */
    std::string get_url_str()const noexcept{return url_;}

    /**
      * @brief Returns the url the environment is using
      */
    std::string_view get_url()const noexcept{return url_;}

    /**
      * @brief version
      */
    std::string_view version()const noexcept{return version_;}


    /**
     * @brief name of the environment
     *
     */
    std::string_view env_name()const noexcept{return name_;}

    /**
     * @brief The number of agents acting in the environment
     *
     */
    uint_t n_agents()const noexcept{return n_agents_;}

    /**
      * \brief Reset the environment always using the same seed
      */
    time_step_type reset(const std::unordered_map<std::string, std::any>& options = std::unordered_map<std::string, std::any>()){
        return reset(DEFAULT_ENV_SEED, options);}

protected:

    /**
     * @brief Constructor
     */
    EnvBase(const std::string& url, const std::string& name=INVALID_STR);

    /**
     * @brief Helper function to set the version. To be called only when the make is called
     */
    void set_version_(const std::string& version )noexcept{version_ = version;}


    void invalidate_is_created_flag_()noexcept{is_created_ = false;}

    /**
      * \brief mark the environment as created
      */
    void make_created_()noexcept{is_created_= true;}
private:

    bool is_created_;
    uint_t n_agents_;
    const std::string url_;
    std::string version_;
    const std::string name_;
};


template<typename TimeStepType, typename ActionType>
EnvBase<TimeStepType, ActionType>::EnvBase(const std::string& url, const std::string& name)
:
is_created_(false),
n_agents_(1),
url_(url),
version_(),
name_(name)

{}

}
}

#endif // ENV_BASE_H
