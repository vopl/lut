#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/coro.hpp"
#include "lut/async/impl/intrusiveQueue.hpp"

#include <map>
#include <thread>
#include <mutex>

namespace lut { namespace async { namespace impl
{
    class Thread;

    class Scheduler
    {
    public:
        using Task = std::function<void()>;
    public:
        Scheduler();
        ~Scheduler();

        ThreadReleaseResult threadRelease(const std::thread::id &id);

    public:
        bool threadEntry_init(Thread *thread);
        bool threadEntry_utilize(Thread *thread);
        void threadEntry_sleep(Thread *thread, std::unique_lock<std::mutex> &mtx);
        bool threadEntry_deinit(Thread *thread);

    public:
        void spawn(const Task &code);
        void spawn(Task &&code);

    public:
        void coroEntry_deactivateAndStayEmpty(Coro *coro);
        void coroEntry_deactivateAndStayReady(Coro *coro);


    private:
        void enqueuePerThreadCoros(Thread *thread);

    private://threads
        std::mutex _threadsMtx;
        typedef std::map<std::thread::id, Thread *> TMThreads;
        TMThreads _threads;
        std::condition_variable _threadsCv;

    private:
        IntrusiveQueue<Coro> _coroListReady;
        IntrusiveQueue<Coro> _coroListEmpty;
    };
}}}

#endif
