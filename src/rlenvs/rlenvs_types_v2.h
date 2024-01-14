#ifndef RLENVS_TYPES_V2_H
#define RLENVS_TYPES_V2_H

#include "rlenvs/rlenvscpp_config.h"

#ifdef USE_PYTORCH
#include <torch/torch.h>
#endif

#include <string>
#include <cstddef>


namespace rlenvs_cpp {


///
/// \brief real_t
///
typedef double real_t;

///
/// \brief uint_t
///
typedef std::size_t uint_t;

///
/// \brief invalid_uint
///
//constexpr uint_t INVALID_UINT = static_cast<uint_t>(-1);

///
/// \brief Invalid string
///
const std::string INVALID_STR = std::string("INVALID");

#ifdef USE_PYTORCH
typedef torch::Tensor torch_tensor_t;
#endif

}

#endif // TYPES_H

