#ifndef TASK_BASE_H
#define TASK_BASE_H

#include "gymfcpp/gymfcpp_types.h"
#include "gymfcpp/gymfcpp_consts.h"

#include <boost/noncopyable.hpp>
#include <string>

namespace rlenvs{

using namespace gymfcpp;

namespace detail{

///
/// \brief Base class for task execution. A task cannot be
/// copied not copy assigned. It can only be moved
///
class task_base
{

public:

    /// \brief A useful enumeration describing the state at which the
    /// task is at. Upon construction task is at PENDING state.
    enum class TaskState{PENDING, STARTED, INTERRUPTED, STOPPED, INTERRUPTED_BY_EXCEPTION, FINISHED, UNDEFINED};

    ///
    /// \brief Destructor
    ///
    virtual ~task_base()=default;

    task_base(const task_base&)=delete;
    task_base& operator=(const task_base&)=delete ;

    ///
    /// \brief task_base
    /// \param other
    ///
    task_base(task_base&& other) noexcept;

    ///
    /// \brief operator =
    /// \param other
    /// \return
    ///
    virtual task_base& operator=(task_base&& other) noexcept;

    ///
    /// \brief Set the state of the task
    ///
    void set_state(TaskState state)noexcept{state_ = state;}

    ///
    /// \brief Query the state of the task
    ///
    TaskState get_state()const noexcept{return state_;}

    ///
    /// \brief Execute the task
    ///
    virtual void operator()();

    ///
    /// \brief Reschedule the task. If called after this->operator()
    /// will override any TaskState has been set. By default it sets the
    /// task state to TaskState::PENDING
    ///
    virtual void reschedule()noexcept{set_state(task_base::TaskState::PENDING);}

    ///
    /// \brief Execute the task
    ///
    void execute();

    ///
    /// \brief Returns true if the task has completed
    ///
    bool finished()const;

    ///
    /// \brief Returns true if the taks has children
    ///
    bool has_children()const noexcept{return false;}

    ///
    /// \brief Id of task
    ///
    uint_t get_id()const noexcept{return id_;}

    ///
    /// \brief Set the id of the task
    ///
    void set_id(uint_t id)noexcept{id_ = id;}


protected:

    ///
    /// \brief Constructor
    ///
    explicit task_base(uint_t id = INVALID_ID) noexcept;

    ///
    /// \brief Function to overrided by defived classes.
    /// It actually executes the compuational task
    ///
    virtual void run_()=0;

    /// \brief The state of the task. Upon creation the
    /// state is TaskState::PENDING
    TaskState state_;

    /// \brief The id of the task
    uint_t id_;
};

inline
void
task_base::execute(){
    (*this)();
}

}
}
#endif // TASK_BASE_H
