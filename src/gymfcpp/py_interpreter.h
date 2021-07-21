#ifndef PY_INTERPRETER_H
#define PY_INTERPRETER_H

#include "py_obj_base.h"
#include "types.h"
#include <boost/python.hpp>

namespace pfcpp {

///
class PyObjBase;

///
/// \brief The PyInterpreter class. This is a simple wrapper
/// over boost::python functions
///
class PyInterpreter
{
   public:

    ///
    /// \brief PyInterpreter
    ///
    PyInterpreter(bool on_start=false);

    ///
    ///
    ///
    ~PyInterpreter();

    ///
    /// \brief start the interpreter
    ///
    void start();

    ///
    /// \brief finalize
    ///
    void finalize();

    ///
    /// \brief import the module
    ///
    PyObjBase import(str_t name);

    ///
    ///
    ///
    PyObjBase eval(str_t expression, obj_t globals = obj_t(), obj_t locals = obj_t());

    ///
    ///
    ///
    PyObjBase exec(str_t code, obj_t globals = obj_t(), obj_t locals = obj_t());

    ///
    ///
    ///
    PyObjBase exec_file(str_t filename, obj_t globals = obj_t(), obj_t locals = obj_t());

private:

    ///
    /// \brief started_
    ///
    bool started_;


};

}

#endif // PY_INTERPRETER_H
