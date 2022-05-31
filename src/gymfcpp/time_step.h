#ifndef TIME_STEP_H
#define TIME_STEP_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step_type.h"

#include <string>
#include <any>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <ostream>

namespace rlenvs_cpp {


///
/// \brief The TimeStep class
///
template<typename StateTp>
class TimeStep
{
public:

    ///
    /// \brief state_ Type of the state
    ///
    typedef StateTp state_t;

    ///
    /// \brief TimeStep
    ///
    TimeStep();

    ///
    /// \brief TimeStep. Constructor
    ///
    TimeStep(TimeStepTp type, real_t reward, state_t obs);

    ///
    /// \brief TimeStep. Constructor
    ///
    TimeStep(TimeStepTp type, real_t reward, state_t obs, std::unordered_map<std::string, std::any>&& extra);

    ///
    /// \brief TimeStep
    /// \param other
    ///
    TimeStep(const TimeStep& other);

    ///
    /// \brief TimeStep
    /// \param other
    ///
    TimeStep(TimeStep&& other)noexcept;

    ///
    /// \brief operator =
    /// \param other
    /// \return
    ///
    TimeStep& operator=(TimeStep&& other)noexcept;
    
    ///
    /// \brief first
    /// \return
    ///
    bool first()const noexcept{return type_._to_index() == TimeStepTp::FIRST;}

    ///
    /// \brief mid
    /// \return
    ///
    bool mid()const noexcept{return type_._to_index() == TimeStepTp::MID;}

    ///
    /// \brief last
    /// \return
    ///
    bool last()const noexcept{return type_._to_index() == TimeStepTp::LAST;}

    ///
    /// \brief type
    /// \return
    ///
    TimeStepTp type()const noexcept{return type_;}

    ///
    /// \brief observation
    /// \return
    ///
    state_t observation()const{return obs_;}

    ///
    /// \brief reward
    /// \return
    ///
    real_t reward()const noexcept{return reward_;}

    ///
    /// \brief done
    /// \return
    ///
    bool done()const noexcept{return type_._to_index() == TimeStepTp::LAST;}

    ///
    /// \brief clear
    ///
    void clear()noexcept;

    ///
    /// \brief get_extra
    ///
    template<typename T>
    const T& get_extra(std::string name)const;

    ///
    /// \brief info
    /// \return
    ///
    const std::unordered_map<std::string, std::any>& info()const noexcept{return extra_;}

    ///
    /// \brief info
    /// \return
    ///
    std::unordered_map<std::string, std::any>& info()noexcept{return extra_;}

private:

    ///
    /// \brief type_
    ///
    TimeStepTp type_;

    ///
    /// \brief reward_
    ///
    real_t reward_;

    ///
    /// \brief obs_
    ///
    state_t obs_;

    ///
    /// \brief extra_
    ///
    std::unordered_map<std::string, std::any> extra_;

};

template<typename StateTp>
TimeStep<StateTp>::TimeStep()
    :
      type_(TimeStepTp::INVALID_TYPE),
      reward_(0.0),
      obs_(),
      extra_()
{}

template<typename StateTp>
TimeStep<StateTp>::TimeStep(TimeStepTp type, real_t reward, state_t obs)
    :
      type_(type),
      reward_(reward),
      obs_(obs),
      extra_()
{}

template<typename StateTp>
TimeStep<StateTp>::TimeStep(TimeStepTp type, real_t reward, state_t obs, std::unordered_map<std::string, std::any>&& extra)
    :
    type_(type),
    reward_(reward),
    obs_(obs),
    extra_(extra)
{}

template<typename StateTp>
TimeStep<StateTp>::TimeStep(const TimeStep& other)
    :
      type_(other.type_),
      reward_(other.reward_),
      obs_(other.obs_),
      extra_(other.extra_)
{}

template<typename StateTp>
TimeStep<StateTp>::TimeStep(TimeStep&& other)noexcept
    :
      type_(other.type_),
      reward_(other.reward_),
      obs_(other.obs_),
      extra_(other.extra_)
{
    other.clear();
}

template<typename StateTp>
TimeStep<StateTp>&
TimeStep<StateTp>::operator=(TimeStep&& other)noexcept{

    type_ = other.type_;
    reward_ = other.reward_;
    obs_ = other.obs_;
    extra_ = other.extra_;
    other.clear();
    return *this;
}

template<typename StateTp>
void
TimeStep<StateTp>::clear()noexcept{

    type_ = TimeStepTp::INVALID_TYPE;
    reward_ = 0.0;
    obs_ = state_t();
    extra_.clear();
}

template<typename StateTp>
template<typename T>
const T&
TimeStep<StateTp>::get_extra(std::string name)const{

    auto itr = extra_.find(name);

    if(itr == extra_.end()){
        throw std::logic_error("Property " + name + " does not exist");
    }

    return std::any_cast<const T&>(itr->second);
}


template<typename StateTp>
inline
std::ostream& operator<<(std::ostream& out, const TimeStep<StateTp>& step){

    out<<"Step type.... "<<gymfcpp::to_string(step.type())<<std::endl;
    out<<"Reward..... .."<<step.reward()<<std::endl;
    out<<"Observation.. "<<step.observation()<<std::endl;
    return out;
}


}

#endif // TIME_STEP_H
