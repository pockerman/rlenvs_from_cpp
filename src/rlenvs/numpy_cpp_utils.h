#ifndef NUMPY_CPP_UTILS_H
#define NUMPY_CPP_UTILS_H

#include "rlenvs/rlenvs_types.h"
#include "rlenvs/rlenvscpp_config.h"

#ifdef GYMFCPP_DEBUG
#include <cassert>
#endif

#include <random>
#include <vector>

namespace rlenvs_cpp{

///
/// \detailed Various utilities from numpy implemented in C++
///
///


///
/// \brief linspace. Numpy documentation:
/// Implementation taken from: https://stackoverflow.com/questions/27028226/python-linspace-in-c
///
template<typename T>
std::vector<real_t> linspace(T start_in, T end_in, uint_t num_in)
{

  std::vector<real_t> linspaced;

  auto start = static_cast<real_t>(start_in);
  auto end =   static_cast<real_t>(end_in);
  auto num =   num_in;

  if (num == 0) {
      return linspaced;
  }

  if (num == 1) {
      linspaced.push_back(start);
      return linspaced;
   }

  auto delta = (end - start) / (num - 1);

  for(uint_t i=0; i < num-1; ++i){
      linspaced.push_back(start + delta * i);
  }

  // I want to ensure that start and end
  // are exactly the same as the input
  linspaced.push_back(end);
  return linspaced;
}

///
/// \brief digitize. Simplified numpy.digitize. Numpy documentation
/// can be found here: https://numpy.org/doc/stable/reference/generated/numpy.digitize.html
/// The SerialContainer is expected to be sorted in ascending order.
///
template<typename T, typename SerialContainer>
uint_t
digitize(const T x, const SerialContainer& container){

#ifdef GYMFCPP_DEBUG
    assert(container.empty() == false && "The given container is empty");
#endif

    // out of min boundary
    if(x < container[0]){
        return 0;
    }

    if( x > container.back()){
        return container.size();
    }

    auto bin = 1;
    for(uint_t i=1; i<container.size(); ++i){

        // we found the bin index
        if((container[i - 1] <= x) && ( x < container[i]) ){
            break;
        }

        bin += 1;
    }

    return bin;
}

///
///
///
template<typename SerialVector, typename T>
std::vector<T>
to_1d_from_2d(uint_t dim1, uint_t dim2, const SerialVector& v){

#ifdef GYMFCPP_DEBUG
    assert(dim1 == v.size() && "Invalid vector first dimension != dim1");
    assert(dim2 == v[0].size() && "Invalid vector second dimension != dim2");
#endif

    std::vector<T> result(dim1 * dim2);
    auto counter = 0;
    for(uint_t i=0; i<dim1; ++i){
        for(uint_t j=0; j<dim2; ++j){
            result[counter] = v[i][j];
            counter += 1;
        }
    }
    return result;
}

///
///
///
template<typename SerialVector, typename T>
std::vector<T>
to_1d_from_3d(uint_t dim1, uint_t dim2, uint_t dim3, const SerialVector& v){

#ifdef GYMFCPP_DEBUG
    assert(dim1 == v.size() && "Invalid vector first dimension != dim1");
    assert(dim2 == v[0].size() && "Invalid vector second dimension != dim2");
    assert(dim3 == v[0][0].size() && "Invalid vector third dimension != dim3");
#endif

    std::vector<T> result(dim1 * dim2 * dim2);
    auto counter = 0;
    for(uint_t i=0; i<dim1; ++i){
        for(uint_t j=0; j<dim2; ++j){
            for(uint_t k=0; k<dim3; ++k)
            result[counter++] = v[i][j][k];
        }
    }
    return result;
}

///
///
///
template<typename SerialVector>
void
add_uniform_noise(SerialVector& vector, uint_t seed){

   std::mt19937 gen(seed);
   std::uniform_real_distribution<> real_dist_(0.0, 1.0);

   for(uint_t i=0; i<vector.size(); ++i){
       vector[i] += real_dist_(gen);
   }
}

///
///
///
template<typename SerialVector>
void
add_uniform_noise(SerialVector& vector, uint_t seed, real_t scale){


   add_uniform_noise(vector, seed);

   for(uint_t i=0; i<vector.size(); ++i){
       vector[i] /= scale;
   }
}



}

#endif // NUMPY_CPP_UTILS_H
