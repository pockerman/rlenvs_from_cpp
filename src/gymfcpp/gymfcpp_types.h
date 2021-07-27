#ifndef GYMFCPP_TYPES_H
#define GYMFCPP_TYPES_H

#include <boost/python.hpp>
#include <cstddef>

namespace gymfcpp {

///
/// \brief str_t
///
typedef boost::python::str str_t;

///
/// \brief obj_t
///
typedef boost::python::api::object obj_t;

///
/// \brief real_t
///
typedef double real_t;

///
/// \brief uint_t
///
typedef std::size_t uint_t;

}

#endif // TYPES_H
