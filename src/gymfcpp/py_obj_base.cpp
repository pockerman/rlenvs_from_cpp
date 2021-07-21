#include "py_obj_base.h"

namespace pfcpp{


PyObjBase::PyObjBase()
    :
      obj_()
{}
PyObjBase::PyObjBase(obj_t&& obj)
    :
      obj_(obj)
{}

}
