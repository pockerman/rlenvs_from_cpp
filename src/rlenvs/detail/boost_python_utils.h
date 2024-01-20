#ifndef BOOST_PYTHON_UTILS_H
#define BOOST_PYTHON_UTILS_H

#include <boost/python.hpp>

namespace rlenvs_cpp{
namespace detail{


template<typename BoostPyType, typename BoostPyNamespace, typename KeyType>
BoostPyType extract_boost_type(BoostPyNamespace py_namespace, const KeyType& key){
    return boost::python::extract<BoostPyType>(py_namespace[key]);
}

}
}

#endif