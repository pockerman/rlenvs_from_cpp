#ifndef TOY_TEXT_BASE_ENV_H
#define TOY_TEXT_BASE_ENV_H

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/rlenvscpp_config.h"
#include "rlenvs/envs/gymnasium/gymnasium_env_base.h"

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <any>
#include <boost/noncopyable.hpp>

#ifdef RLENVSCPP_DEBUG
#include <cassert>
#endif

namespace rlenvs_cpp{
namespace envs{
namespace gymnasium {

template<typename TimeStepType>
class ToyTextEnvBase: public GymnasiumEnvBase<TimeStepType>{
public:

    typedef TimeStepType time_step_type;

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    virtual ~ToyTextEnvBase()=default;

    ///
    /// \brief P
    /// \param sidx
    /// \param aidx
    ///
    dynamics_t p(uint_t sidx, uint_t aidx)const;

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) = 0;


    /**
     * @brief Synchronize the environment
     *
     */
    void sync(const std::unordered_map<std::string, std::any>& /*options*/=std::unordered_map<std::string, std::any>()){}


protected:

    ///
    /// \brief Constructor
    ///
    ToyTextEnvBase(const std::string& url);

    ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const=0;

    ///
    /// \brief build the time step from the server response
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response)const=0;

};

template<typename TimeStepType>
ToyTextEnvBase<TimeStepType>::ToyTextEnvBase(const std::string& url)
:
GymnasiumEnvBase<TimeStepType>(url)
{}


template<typename TimeStepType>
typename ToyTextEnvBase<TimeStepType>::dynamics_t
ToyTextEnvBase<TimeStepType>::p(uint_t sidx, uint_t aidx)const{

#ifdef RLENVSCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif

    const std::string url(this->get_url());
    const auto request_url = url + "/dynamics?stateId="+std::to_string(sidx)+"&actionId="+std::to_string(aidx);
    http::Request request{request_url};
    const auto response = request.send("GET");

    if(response.status.code != 200){
        throw std::runtime_error("Environment server failed to return  environment dynamics");
    }

    return build_dynamics_from_response_(response);
}


} // gymnasium
} // envs
} // rlenvs_cpp
#endif
