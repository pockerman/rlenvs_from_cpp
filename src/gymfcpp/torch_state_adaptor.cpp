#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/torch_state_adaptor.h"

#ifdef USE_PYTORCH

namespace rlenvs{
namespace torch_utils{


torch_tensor_t
rlenvs::torch_utils::TorchStateAdaptor::operator()(const std::vector<real_t>& data){
    return torch::tensor(data);
}

torch_tensor_t
rlenvs::torch_utils::TorchStateAdaptor::operator()(const std::vector<int>& data){

    return torch::tensor(data);
};

}

}
#endif


