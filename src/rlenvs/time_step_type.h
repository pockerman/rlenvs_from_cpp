#ifndef TIME_STEP_TYPE_H
#define TIME_STEP_TYPE_H

#include "rlenvs/extern/enum.h"
#include <string>

namespace rlenvs_cpp
{

///
/// \brief The TimeStepTp enum
///

BETTER_ENUM(TimeStepTp, char, FIRST=0, MID, LAST, INVALID_TYPE);

///
/// \brief to_string
/// \param type
/// \return
///
inline
std::string to_string(TimeStepTp type){return type._to_string();}

///
/// \brief Create a time step type from the given int
///
TimeStepTp time_step_type_from_int(int type);

}

#endif // TIME_STEP_TYPE_H
