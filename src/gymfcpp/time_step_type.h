#ifndef TIME_STEP_TYPE_H
#define TIME_STEP_TYPE_H

#include "gymfcpp/extern/enum.h"
#include <string>

namespace gymfcpp
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

}

#endif // TIME_STEP_TYPE_H
