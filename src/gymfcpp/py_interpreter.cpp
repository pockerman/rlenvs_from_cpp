#include "config.h"
#include "py_interpreter.h"
#include "py_obj_base.h"

#ifdef PFCPP_DEBUG
#include <cassert>
#endif

namespace pfcpp {

PyInterpreter::PyInterpreter(bool start_on)
    :
      started_(start_on)
{
    try{
        if(start_on){
            start();
        }
    }
    catch(...){

        started_  = false;
        throw;
    }
}

PyInterpreter::~PyInterpreter(){

    finalize();
}


void
PyInterpreter::start(){

    // create the __main__ module
    Py_Initialize();
    started_ = true;

}

void
PyInterpreter::finalize(){

}

PyObjBase
PyInterpreter::import(str_t module_name){

#ifdef PFCPP
    assert(started_ && "Interpreter has not been started");
#endif

    return PyObjBase(boost::python::import(module_name));
}

PyObjBase 
PyInterpreter::eval(str_t expression, obj_t globals, obj_t locals ){
    
#ifdef PFCPP
    assert(started_ && "Interpreter has not been started");
#endif

    return PyObjBase(boost::python::eval(expression, globals, locals));

}

PyObjBase
PyInterpreter::exec(str_t code, obj_t globals, obj_t locals){

#ifdef PFCPP
    assert(started_ && "Interpreter has not been started");
#endif

    return PyObjBase(boost::python::exec(code, globals, locals));

}

PyObjBase
PyInterpreter::exec_file(str_t filename, obj_t globals, obj_t locals ){

#ifdef PFCPP
    assert(started_ && "Interpreter has not been started");
#endif

    return PyObjBase(boost::python::exec_file(filename, globals, locals));


}

}
