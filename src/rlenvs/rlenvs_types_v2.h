#ifndef RLENVS_TYPES_V2_H
#define RLENVS_TYPES_V2_H

#include "rlenvs/rlenvscpp_config.h"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#ifdef USE_PYTORCH
#include <torch/torch.h>
#endif

#include <string>
#include <cstddef>



namespace rlenvscpp {

///
/// \brief real_t
///
typedef double real_t;


///
/// \brief float
///
typedef float float_t;

///
/// \brief integer type
///
typedef int int_t;

///
/// \brief uint_t
///
typedef std::size_t uint_t;


template<typename T>
using DynMat = Eigen::MatrixX<T>;


template<typename T>
using DynVec = Eigen::RowVectorX<T>;

///
///
///
using FloatVec = DynVec<float_t>;

///
///
///
using RealVec = DynVec<real_t>;

///
/// \brief invalid_uint
///
//constexpr uint_t INVALID_UINT = static_cast<uint_t>(-1);


#ifdef USE_PYTORCH
typedef torch::Tensor torch_tensor_t;
#endif

}

#endif // TYPES_H

