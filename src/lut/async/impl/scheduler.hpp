#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/context.hpp"
#include "lut/async/impl/coro.hpp"
#include "lut/async/impl/coroContainer.hpp"
#include "lut/async/impl/task.hpp"

#include <vector>
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
        void threadEntry_utilize(Thread *thread);
        bool threadEntry_deinit(Thread *thread);

    public:
        void spawn(Task &&code);
        void yield();

    public:
        void coroEntry_stayEmptyAndDeactivate(Coro *coro, Thread *thread);
        void coroEntry_stayReadyAndDeactivate(Coro *coro, Thread *thread);


    public:
        void enqueuePerThreadCoros(Thread *thread);

        Coro *fetchReadyCoro4Thread(Thread *thread);

    private://threads
        std::mutex _threadsMtx;
        typedef std::vector<Thread *> TVThreads;
        TVThreads _threads;

    private:
        CoroContainer _coroListReady;
        CoroContainer _coroListEmpty;
    };
}}}

#endif
