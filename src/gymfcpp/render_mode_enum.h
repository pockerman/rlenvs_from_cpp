#ifndef RENDER_MODE_ENUM_H
#define RENDER_MODE_ENUM_H

#include "gymfcpp/extern/enum.h"

#include<string>

namespace gymfcpp {

///
/// \brief The RenderModeType enum
///
BETTER_ENUM(RenderModeType, char, HUMAN=0, RGB_ARRAY, INVALID);


///
/// \brief to_string
/// \param type
/// \return

inline
std::string to_string(RenderModeType type){return type._to_string();}


}

#endif // RENDER_MODE_ENUM_H
