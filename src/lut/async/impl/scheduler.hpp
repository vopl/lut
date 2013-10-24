#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/ctx/root.hpp"
#include "lut/async/impl/ctx/coro.hpp"
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
        void coroEntry_stayEmptyAndDeactivate(ctx::Coro *coro, Thread *thread);
        void coroEntry_stayReadyAndDeactivate(ctx::Coro *coro, Thread *thread);


    public:
        void enqueuePerThreadCoros(Thread *thread);

        ctx::Coro *fetchReadyCoro4Thread(Thread *thread);

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
