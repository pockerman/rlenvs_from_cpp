#include "gymfcpp/gymfcpp_config.h"
#include "gymfcpp/torch_state_adaptor.h"

#ifdef USE_PYTORCH

namespace rlenvs{
namespace torch_utils{


torch_tensor_t
TorchStateAdaptor::operator()(real_t value)const{
    return this->operator()(std::vector<real_t>(1, value));
}

torch_tensor_t
TorchStateAdaptor::operator()(const std::vector<real_t>& data)const{
    return torch::tensor(data);
}

torch_tensor_t
TorchStateAdaptor::operator()(const std::vector<int>& data)const{

    return torch::tensor(data);
};

TorchStateAdaptor::value_type
TorchStateAdaptor::stack(const std::vector<value_type>& values)const{

    return torch::stack(values, 0);
}

}

}
#endif


