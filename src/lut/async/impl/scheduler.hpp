#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include <map>
#include <thread>
#include <mutex>

namespace lut { namespace async { namespace impl
{
    class Thread;

    class Scheduler
    {
    public:
        Scheduler();
        ~Scheduler();

        ThreadReleaseResult threadRelease(const std::thread::id &id);

    public:
        bool threadEntry_init(Thread *thread);
        bool threadEntry_utilize(Thread *thread);
        bool threadEntry_deinit(Thread *thread);

    private://threads
        std::mutex _threadsMtx;
        typedef std::map<std::thread::id, Thread *> TMThreads;
        TMThreads _threads;

    };
}}}

#endif
