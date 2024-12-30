#include "rlenvs/rlenvs_types_v2.h"

namespace rlenvscpp{
namespace utils{
namespace maths{
	
///
/// \brief Returns the sign of the expression
///
template<typename T>
int_t sign(const T& expr){

    if(expr < 0){
        return -1;
    }

    return 1;
}


template<typename T>
int_t sign(const T& expr1, const T& expr2){
    return sign(expr1*expr2);
}

///
/// \brief Compute the sum of squares
/// of the elements of the given vector
///
template<typename T>
T sum_sqr(const DynVec<T>& vec){

	T sum_ = T(0);
	for(uint_t i = 0; i<vec.size(); ++i){
		sum_ += vec[i] * vec[i];
	}
	
	return sum_;
}

template<typename T>
T sqr(const T& v){
	return v*v;
}


}
}
}