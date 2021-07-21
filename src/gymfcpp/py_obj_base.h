#ifndef PY_OBJ_BASE_H
#define PY_OBJ_BASE_H



#include "types.h"


namespace pfcpp{


///
/// \brief The PyObjBase class. Wrapper to
/// boost::python::api::object
///
class PyObjBase
{

public:

    ///
    /// \brief PyObjBase
    ///
    PyObjBase();

    ///
    /// \brief PyObjBase
    /// \param obj
    ///
    PyObjBase(obj_t&& obj);

    ///
    /// \brief get_raw
    /// \return
    ///
    obj_t& get_raw(){return obj_;}

    ///
    /// \brief get_raw
    /// \return
    ///
    const obj_t& get_raw()const{return obj_;}

protected:

    ///
    /// \brief obj_
    ///
    obj_t obj_;

};

}

#endif // PY_OBJ_BASE_H
