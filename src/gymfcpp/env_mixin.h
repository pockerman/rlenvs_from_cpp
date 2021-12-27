#ifndef ENV_MIXIN_H
#define ENV_MIXIN_H

#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/render_mode_enum.h"
#include "gymfcpp/time_step_type.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <type_traits>
#include <iostream>

namespace gymfcpp
{

///
/// \brief The EnvMixin class
///
template<typename EnvImpl>
struct EnvMixin //: private boost::noncopyable
{

    typedef EnvImpl env_impl_data_type;

    ///
    /// \brief time_step_t
    ///
    typedef typename env_impl_data_type::time_step_type time_step_type;


    static_assert(std::is_default_constructible<time_step_type>::value, "Time step type is not default constructible");

    ///
    /// \brief EnvMixin
    /// \param version
    /// \param gym_namespace
    ///
    EnvMixin(std::string version, obj_t gym_namespace);

    ///
    /// \brief reset
    /// \return
    ///
    time_step_type reset();

    ///
    /// \brief close. Close down the python environment
    ///
    void close();

    ///
    /// \brief full_name
    /// \return
    ///
    std::string full_name()const noexcept{return EnvImpl::name + "-" + version;}

    ///
    /// \brief render
    /// \param render_mode
    ///
    void render(RenderModeType render_mode);

    ///
    /// \brief py_env_name
    ///
    std::string py_env_name;

    ///
    /// \brief py_reset_result_name
    ///
    std::string py_reset_result_name;

    ///
    /// \brief py_step_result_name
    ///
    std::string py_step_result_name;

    ///
    /// \brief py_state_name
    ///
    std::string py_state_name;

    ///
    /// \brief version
    /// \return
    ///
    std::string version;

    ///
    /// \brief is_created
    ///
    bool is_created;

    ///
    /// \brief idx
    ///
    uint_t idx;

    ///
    /// \brief gym_namespace_
    ///
    obj_t gym_namespace;

    ///
    /// \brief env
    ///
    obj_t env;

    ///
    /// \brief current_state
    ///
    time_step_type current_state;

};

template<typename EnvImpl>
EnvMixin<EnvImpl>::EnvMixin(std::string version_, obj_t gym_namespace_)
    :
     py_env_name(),
     py_reset_result_name(),
     py_step_result_name(),
     py_state_name(),
     version(version_),
     is_created(false),
     idx(0),
     gym_namespace(gym_namespace_),
     env(),
     current_state()
{}

template<typename EnvImpl>
void
EnvMixin<EnvImpl>::close(){

    if(!is_created){
        return;
    }

    auto str = py_env_name + ".close()\n";
    boost::python::exec(str.c_str(), gym_namespace);
    is_created = false;
}

template<typename EnvImpl>
typename EnvMixin<EnvImpl>::time_step_type
EnvMixin<EnvImpl>::reset(){

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
#endif

    auto cpp_str = py_reset_result_name + " = ";
    cpp_str += py_env_name + ".reset()";

    // reset the python environment
    boost::python::exec(cpp_str.c_str(), gym_namespace);

    auto obs = EnvImpl::extract_state_from_reset(gym_namespace, py_state_name, py_reset_result_name);
    current_state = time_step_type(TimeStepTp::FIRST, 0.0, obs);
    return current_state;
}

template<typename EnvImpl>
void
EnvMixin<EnvImpl>::render(RenderModeType render_mode){

#ifdef GYMFCPP_DEBUG
    assert(is_created && "Environment has not been created");
#endif

    auto str = "screen = " + py_env_name + ".render(mode='" + gymfcpp::to_string(render_mode) + "')\n";
    boost::python::exec(str.c_str(), gym_namespace);

}

}

#endif // ENV_BASE_H
