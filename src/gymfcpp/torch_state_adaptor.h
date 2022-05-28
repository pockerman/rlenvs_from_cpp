#ifndef TORCH_STATE_ADAPTOR_H
#define TORCH_STATE_ADAPTOR_H

#include "gymfcpp/gymfcpp_config.h"

#ifdef USE_PYTORCH

#include "gymfcpp/gymfcpp_types.h"
#include <vector>

namespace rlenvs{
namespace torch_utils {

using namespace gymfcpp;

struct TorchStateAdaptor{


    torch_tensor_t operator()(const std::vector<real_t>& data);
    torch_tensor_t operator()(const std::vector<int>& data);
};



}
}

#endif
#endif // TORCH_STATE_ADAPTOR_H
