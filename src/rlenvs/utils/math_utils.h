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


}
}
}