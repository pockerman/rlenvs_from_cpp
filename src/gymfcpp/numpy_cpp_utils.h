#ifndef NUMPY_CPP_UTILS_H
#define NUMPY_CPP_UTILS_H

#include "gymfcpp/gymfcpp_types.h"
#include <vector>

namespace gymfcpp{

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

}

#endif // NUMPY_CPP_UTILS_H
