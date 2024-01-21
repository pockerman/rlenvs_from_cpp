#ifndef TOY_TEXT_BASE_ENV_H
#define TOY_TEXT_BASE_ENV_H

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
class ToyTextEnvBase: private boost::noncopyable{
public:

    typedef TimeStepType time_step_type;

    ///
    /// \brief dynamics_t
    ///
    typedef std::vector<std::tuple<real_t, uint_t, real_t, bool>> dynamics_t;

    ///
    /// \brief ~FrozenLake. Destructor.
    ///
    virtual ~ToyTextEnvBase();

    ///
    /// \brief Query the environment server is the environment has been created
    ///
    bool is_alive()const noexcept;

    ///
    /// \brief is_created Returns true is make has been called successfully
    ///
    bool is_created()const noexcept{return is_created_;}

    ///
    /// \brief mark the environment as created
    ///
    void make_created()noexcept{is_created_= true;}

    ///
    /// \brief Returns the url the environment is using
    ///
    std::string_view get_url()const noexcept{return url_;}

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
    ToyTextEnvBase(const std::string& url);


    time_step_type& get_current_time_step_()noexcept{return current_state_;}
    const time_step_type& get_current_time_step_()const noexcept{return current_state_;}


    ///
    /// \brief build the dynamics from response
    ///
    virtual dynamics_t build_dynamics_from_response_(const http::Response& response)const=0;

    ///
    /// \brief build the time step from the server response
    ///
    virtual time_step_type create_time_step_from_response_(const http::Response& response)const=0;


    ///
    /// \brief The urls of the server
    ///
    const std::string url_;

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
ToyTextEnvBase<TimeStepType>::ToyTextEnvBase(const std::string& url)
:
url_(url),
is_created_(false),
current_state_()
{}

template<typename TimeStepType>
ToyTextEnvBase<TimeStepType>::~ToyTextEnvBase(){

    close();
}

template<typename TimeStepType>
bool
ToyTextEnvBase<TimeStepType>::is_alive()const noexcept{
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
ToyTextEnvBase<TimeStepType>::close(){

     if(!is_created_){
        return;
    }

    http::Request request{url_ + "/close"};
    const auto response = request.send("POST");
    is_created_ = false;

}

template<typename TimeStepType>
typename ToyTextEnvBase<TimeStepType>::dynamics_t
ToyTextEnvBase<TimeStepType>::p(uint_t sidx, uint_t aidx)const{

#ifdef RLENVSCPP_DEBUG
    assert(this->is_created_ && "Environment has not been created");
#endif

    const auto request_url = url_ + "/dynamics?stateId="+std::to_string(sidx)+"&actionId="+std::to_string(aidx);
    http::Request request{request_url};
    const auto response = request.send("GET");

    if(response.status.code != 200){
        throw std::runtime_error("Environment server failed to return  environment dynamics");
    }

    return build_dynamics_from_response_(response);
}

template<typename TimeStepType>
typename ToyTextEnvBase<TimeStepType>::time_step_type
ToyTextEnvBase<TimeStepType>::reset(uint_t seed){

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
#endif