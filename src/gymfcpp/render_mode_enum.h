#ifndef RENDER_MODE_ENUM_H
#define RENDER_MODE_ENUM_H

#include "gymfcpp/extern/enum.h"

#include<string>

namespace rlenvs_cpp {

///
/// \brief The RenderModeType enum
///
BETTER_ENUM(RenderModeType, char, human=0, rgb_array, ansi, INVALID_TYPE);


///
/// \brief to_string.  Returns the RenderModeType to its stringrepresentation
/// \param type The RenderModeType to convert
/// \return std::string

inline
std::string to_string(RenderModeType type){return type._to_string();}


}

#endif // RENDER_MODE_ENUM_H
