#ifndef TORCH_STATE_ADAPTOR_H
#define TORCH_STATE_ADAPTOR_H

#include "gymfcpp/gymfcpp_config.h"

#ifdef USE_PYTORCH

#include "gymfcpp/gymfcpp_types.h"
#include <vector>

namespace rlenvs_cpp{
namespace torch_utils {


///
/// \brief The TorchStateAdaptor struct
///
struct TorchStateAdaptor{


    typedef torch_tensor_t value_type;
    typedef torch_tensor_t state_type;

    torch_tensor_t operator()(real_t value)const;
    torch_tensor_t operator()(const std::vector<real_t>& data)const;
    torch_tensor_t operator()(const std::vector<int>& data)const;

    value_type stack(const std::vector<value_type>& values)const;
};



}
}

#endif
#endif // TORCH_STATE_ADAPTOR_H
