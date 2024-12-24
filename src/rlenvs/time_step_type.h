#ifndef TIME_STEP_TYPE_H
#define TIME_STEP_TYPE_H

#include "rlenvs/rlenvs_types_v2.h"
#include "rlenvs/extern/enum.h"
#include <string>
#include <vector>

namespace rlenvscpp
{

///
/// \brief The TimeStepTp enum
///

BETTER_ENUM(TimeStepTp, int, FIRST=0, MID, LAST, INVALID_TYPE);

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

///
/// \brief Utilities for TimeStepTp 
///
struct TimeStepEnumUtils{
		static TimeStepTp time_step_type_from_int(uint_t aidx);
		static std::vector<TimeStepTp> time_step_type_from_int(const std::vector<uint_t>& types);
	};

}

#endif // TIME_STEP_TYPE_H
