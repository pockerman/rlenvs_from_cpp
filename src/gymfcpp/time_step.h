#ifndef TIME_STEP_H
#define TIME_STEP_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/time_step_type.h"

#include <string>
#include <any>
#include <unordered_map>
#include <stdexcept>
#include <ostream>

namespace gymfcpp {

///
/// \brief The TimeStep class
///
class TimeStep
{
public:

    ///
    /// \brief TimeStep
    ///
    TimeStep();

    ///
    ///
    ///
    TimeStep(TimeStepTp type, real_t reward, uint_t obs);

    ///
    ///
    ///
    TimeStep(TimeStepTp type, real_t reward, uint_t obs, std::unordered_map<std::string, std::any>&& extra);

    ///
    /// \brief first
    /// \return
    ///
    bool first()const{return type_ == TimeStepTp::FIRST;}

    ///
    /// \brief mid
    /// \return
    ///
    bool mid()const{return type_ == TimeStepTp::MID;}

    ///
    /// \brief last
    /// \return
    ///
    bool last()const{return type_ == TimeStepTp::LAST;}

    ///
    /// \brief type
    /// \return
    ///
    TimeStepTp type()const{return type_;}

    ///
    /// \brief observation
    /// \return
    ///
    uint_t observation()const{return obs_;}

    ///
    /// \brief reward
    /// \return
    ///
    real_t reward()const{return reward_;}

    ///
    ///
    ///
    template<typename T>
    const T& get_extra(std::string name)const;

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
    uint_t obs_;

    ///
    /// \brief extra_
    ///
    std::unordered_map<std::string, std::any> extra_;

};

template<typename T>
const T&
TimeStep::get_extra(std::string name)const{

    auto itr = extra_.find(name);

    if(itr == extra_.end()){
        throw std::logic_error("Property " + name + " does not exist");
    }

    return std::any_cast<const T&>(itr->second);
}

inline
std::ostream& operator<<(std::ostream& out, const TimeStep& step){

    out<<"Step type.... "<<gymfcpp::to_string(step.type())<<std::endl;
    out<<"Reward..... .."<<step.reward()<<std::endl;
    out<<"Observation.. "<<step.observation()<<std::endl;
    return out;
}


}

#endif // TIME_STEP_H
