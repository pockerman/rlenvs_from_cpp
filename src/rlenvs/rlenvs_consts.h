#ifndef GYMFCPP_CONSTS_H
#define GYMFCPP_CONSTS_H

#include "gymfcpp/gymfcpp_types.h"
#include <string>

namespace rlenvs_cpp {

///
/// \brief PY_ENV_NAME
///
inline const std::string PY_ENV_NAME = "py_env_name";

///
/// \brief PY_RESET_ENV_RESULT_NAME
///
inline const std::string PY_RESET_ENV_RESULT_NAME = "py_reset_result_name";

///
/// \brief PY_STEP_ENV_RESULT_NAME
///
inline const std::string PY_STEP_ENV_RESULT_NAME = "py_step_result_name";

///
/// \brief PY_STATE_NAME
///
inline const std::string PY_STATE_NAME = "py_state_name";

///
/// \brief INVALID_ID
///
inline const uint_t INVALID_ID = static_cast<uint_t>(-1);

}

#endif // GYMFCPP_CONSTS_H
