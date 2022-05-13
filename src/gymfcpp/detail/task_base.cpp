#include "gymfcpp/detail/task_base.h"

namespace rlenvs{
namespace detail {



task_base::task_base(uint_t id)noexcept
    :
    state_(task_base::TaskState::PENDING),
    id_(id)
{}

task_base::task_base(task_base&& other)noexcept
    :
      state_(other.state_),
      id_(other.id_)
{
   other.state_ = TaskState::UNDEFINED;
   other.id_ = INVALID_ID;
}

///
/// \brief operator =
/// \param other
/// \return
///
task_base&
task_base::operator=(task_base&& other)noexcept{

    state_ = other.state_;
    id_ = other.id_;

    other.state_ = TaskState::UNDEFINED;
    other.id_ = INVALID_ID;
    return *this;
}

void
task_base::operator()(){

    try {

        // assume that the task state may have been set and
        // be different than PENDING in this case we don't run
        // Perhaps is better if this throws an exception
        if(get_state() != task_base::TaskState::PENDING)
            return;

        set_state(task_base::TaskState::STARTED);
        run_();
        set_state(task_base::TaskState::FINISHED);
    }
    catch(std::logic_error& error){

        // whatever caused this, we assume that the task was interrupted
        // by an exception
        set_state(task_base::TaskState::INTERRUPTED_BY_EXCEPTION);
    }
    catch (...) {

        //whatever caused this, we assume that the task was interrupted
        set_state(task_base::TaskState::INTERRUPTED_BY_EXCEPTION);
    }
}



bool
task_base::finished()const{

    if(!has_children()){

      return (state_ != task_base::TaskState::PENDING &&
              state_ != task_base::TaskState::STARTED /*&&
              state_ != task_base::TaskState::INTERRUPTED &&
              state_ != task_base::TaskState::INTERRUPTED_BY_EXCEPTION*/ );
    }

    return false;
}

}
}
