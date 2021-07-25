#ifndef TIME_STEP_TYPE_H
#define TIME_STEP_TYPE_H

#include <string>

namespace gymfcpp
{

///
/// \brief The TimeStepTp enum
///
enum class TimeStepTp{FIRST, MID, LAST, INVALID_TYPE};

///
/// \brief to_string
/// \param type
/// \return
///
std::string to_string(TimeStepTp type);

}

#endif // TIME_STEP_TYPE_H
