// SPDX-FileCopyrightText: 2024 <copyright holder> <email>
// SPDX-License-Identifier: Apache-2.0

#ifndef GYMNASIUM_ENV_BASE_H
#define GYMNASIUM_ENV_BASE_H


#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/extern/HTTPRequest.hpp"
#include "rlenvs/rlenvscpp_config.h"
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
class GymnasiumEnvBase: private boost::noncopyable{
public:

    typedef TimeStepType time_step_type;

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    virtual ~GymnasiumEnvBase();

    ///
    /// \brief Query the environment server is the environment has been created
    ///
    bool is_alive()const noexcept;

    ///
    /// \brief is_created Returns true is make has been called successfully
    ///
    bool is_created()const noexcept{return is_created_;}

    ///
    /// \brief Returns the url the environment is using
    ///
    std::string_view get_url()const noexcept{return url_;}

    ///
    /// \brief version
    ///
    std::string_view version()const noexcept{return version_;}

    ///
    /// \brief make. Builds the environment. Optionally we can choose if the
    /// environment will be slippery
    ///
    virtual void make(const std::string& version,
                      const std::unordered_map<std::string, std::any>& options) = 0;

    ///
    /// \brief Reset the environment
    ///
    virtual time_step_type reset(uint_t seed);

    ///
    /// \brief close the environment
    ///
    virtual void close();


protected:

    ///
    /// \brief Constructor
    ///
    GymnasiumEnvBase(const std::string& url);


    time_step_type& get_current_time_step_()noexcept{return current_state_;}
    const time_step_type& get_current_time_step_()const noexcept{return current_state_;}

    ///
    /// \brief Set the version of the environment
    ///
    void set_version(const std::string& v)noexcept{version_ = v;}

    ///
    /// \brief mark the environment as created
    ///
    void make_created()noexcept{is_created_= true;}


    ///
    /// \brief build the time step from the server response
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response)const=0;


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

    ///
    /// \brief current_state
    ///
    time_step_type current_state_;


};

template<typename TimeStepType>
GymnasiumEnvBase<TimeStepType>::GymnasiumEnvBase(const std::string& url)
:
url_(url),
is_created_(false),
current_state_()
{}

template<typename TimeStepType>
GymnasiumEnvBase<TimeStepType>::~GymnasiumEnvBase(){

    close();
}

template<typename TimeStepType>
bool
GymnasiumEnvBase<TimeStepType>::is_alive()const noexcept{
    http::Request request{url_ + "/is-alive"};
    const auto response = request.send("GET");
    const auto str_response = std::string(response.body.begin(), response.body.end());
    auto pos = str_response.find("true");

    if (pos == std::string::npos){
        return false;
    }

    return true;
}

template<typename TimeStepType>
void
GymnasiumEnvBase<TimeStepType>::close(){

     if(!is_created_){
        return;
    }

    http::Request request{url_ + "/close"};
    const auto response = request.send("POST");
    is_created_ = false;

}


template<typename TimeStepType>
typename GymnasiumEnvBase<TimeStepType>::time_step_type
GymnasiumEnvBase<TimeStepType>::reset(uint_t seed){

    if(!is_created_){
#ifdef RLENVSCPP_DEBUG
     assert(this->is_created_ && "Environment has not been created");
#endif
     return time_step_type();
    }

    const auto request_url = url_ + "/reset";
    http::Request request{request_url};

    auto body = std::to_string(seed);
    const auto response = request.send("POST", body);

     if(response.status.code != 202){
        throw std::runtime_error("Environment server failed to reset environment");
    }

    current_state_ = this->create_time_step_from_response_(response);
    return current_state_;

}

} // gymnasium
} // envs
} // rlenvs_cpp

#endif // GYMNASIUMENVBASE_H
