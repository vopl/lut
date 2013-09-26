#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/context.hpp"
#include "lut/async/impl/coro.hpp"
#include "lut/async/impl/intrusiveQueue.hpp"
#include "lut/async/impl/task.hpp"

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
        void threadEntry_utilize(Thread *thread);
        bool threadEntry_deinit(Thread *thread);

    public:
        void spawn(Task &&code);
        void yield();

    public:
        void coroEntry_stayEmptyAndDeactivate(Coro *coro);
        void coroEntry_stayReadyAndDeactivate(Coro *coro);


    public:
        void enqueuePerThreadCoros(Thread *thread);

    private://threads
        std::mutex _threadsMtx;
        typedef std::map<std::thread::id, Thread *> TMThreads;
        TMThreads _threads;

    private:
        IntrusiveQueue<Coro> _coroListReady;
        IntrusiveQueue<Coro> _coroListEmpty;
    };
}}}

#endif
