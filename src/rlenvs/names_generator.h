#ifndef NAMES_GENERATOR_H
#define NAMES_GENERATOR_H

#include <string>

namespace rlenvs_cpp {

///
///
///
inline
std::string get_py_env_name(std::string_view env_name){return "py_" + std::string(env_name);}

///
/// \brief py_step_rslt
/// \param env_name
/// \return
///
inline
std::string get_py_step_rslt_name(std::string_view env_name){return "py_" + std::string(env_name) + "_step_rslt";}

///
/// \brief py_state
/// \param env_name
/// \return
///
inline
std::string get_py_state_name(std::string_view env_name){return "py_" + std::string(env_name) + "_state";}

///
/// \brief py_reset_rslt
/// \param env_name
/// \return
///
inline
std::string get_py_reset_rslt_name(std::string_view env_name){return "py_" + std::string(env_name) + "_reset_rslt";}


}

#endif // NAMES_GENERATOR_H
