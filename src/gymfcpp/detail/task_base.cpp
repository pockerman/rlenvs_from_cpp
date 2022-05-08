#include "kernel/parallel/threading/task_base.h"

#include "kernel/base/config.h"

#ifdef USE_LOG
#include "kernel/utilities/logger.h"
#include <sstream>
#endif


namespace kernel{

TaskBase::TaskBase(uint_t id)
    :
    state_(TaskBase::TaskState::PENDING),
    id_(id),
    name_(KernelConsts::dummy_string())
{}

TaskBase::~TaskBase()
{}

void
TaskBase::operator()(){

    try {

        // assume that the task state may have been set and
        // be different than PENDING in this case we don't run
        // Perhaps is better if this throws an exception
        if(get_state() != TaskBase::TaskState::PENDING)
            return;

        set_state(TaskBase::TaskState::STARTED);
        run();
        set_state(TaskBase::TaskState::FINISHED);

#ifdef USE_LOG
        std::ostringstream message;
        message<<"Task: "<<get_name()<<" finished successfully";
        Logger::log_info(message.str());
#endif
    }
    catch(std::logic_error& error){

#ifdef USE_LOG
        std::ostringstream message;
        message<<"An logic occured whilst running task: "<<this->get_name();
        message<<" what() says: "<<error.what();
        Logger::log_error(message.str());
#endif

        // whatever caused this, we assume that the task was interrupted
        // by an exception
        set_state(TaskBase::TaskState::INTERRUPTED_BY_EXCEPTION);

    }
    catch (...) {

        //whatever caused this, we assume that the task was interrupted
#ifdef USE_LOG
        std::ostringstream message;
        message<<"Task: "<<get_name()<<" was interrupted";
        Logger::log_error(message.str());
#endif
        set_state(TaskBase::TaskState::INTERRUPTED);
    }
}



bool
TaskBase::finished()const{

    if(!has_children()){

      return (state_ != TaskBase::TaskState::PENDING &&
              state_ != TaskBase::TaskState::STARTED /*&&
              state_ != TaskBase::TaskState::INTERRUPTED &&
              state_ != TaskBase::TaskState::INTERRUPTED_BY_EXCEPTION*/ );
    }

    return false;
}

}
