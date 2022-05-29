#ifndef THREAD_JOINER_H
#define THREAD_JOINER_H

#include "boost/noncopyable.hpp"

namespace rlenvs{
namespace detail {

///
/// \brief Class ThreadJoiner is responsible for joining the
/// threads passed to it when it goes out of scope.
/// The class implements the RAII pattern. The class
/// cannot be copied nor copy assigned
///
template<typename ThreadPoolTp>
class thread_joiner: private boost::noncopyable
{

public:

    typedef ThreadPoolTp thread_pool_t;

    ///
    /// \brief Constructor. Initialize with the thread-pool
    /// to manage
    ///
    explicit thread_joiner(ThreadPoolTp& pool);


    ///
    /// \brief Constructor. Responsible for joining
    /// the joinable threads.
    ///
    ~thread_joiner();

    ///
    /// \brief Explicitly join the threads. After this operation
    /// the instance of the ThreadJoiner will be useless
    ///
    void join();

private:

    thread_pool_t& pool_;
};

template<typename ThreadPoolTp>
thread_joiner<ThreadPoolTp>::thread_joiner(ThreadPoolTp& pool)
    :
pool_(pool)
{}


template<typename ThreadPoolTp>
thread_joiner<ThreadPoolTp>::~thread_joiner(){
   join();
}

template<typename ThreadPoolTp>
void
thread_joiner<ThreadPoolTp>::join(){

    for(auto& t:pool_){

        if(t.joinable())
            t.join();
    }
}

}
}
#endif // THREAD_HOINER_H
