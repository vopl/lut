#ifndef _LUT_ASYNC_IMPL_SCHEDULER_HPP_
#define _LUT_ASYNC_IMPL_SCHEDULER_HPP_

#include "lut/async/threadState.hpp"
#include "lut/async/impl/worker/thread.hpp"
#include "lut/async/impl/worker/effort.hpp"

#include <vector>
#include <thread>
#include <mutex>

namespace lut { namespace async { namespace impl
{
    class Scheduler
    {
    public:
        Scheduler();
        ~Scheduler();

        ThreadReleaseResult releaseThread(const std::thread::id &id);

    public:
        bool threadEntry_init(worker::Thread *thread);
        bool threadEntry_deinit(worker::Thread *thread);

        bool threadEntry_provideWork(worker::Thread *thread);
        void threadEntry_workContinued(worker::Thread *thread);

    public:
        void spawn(Task *task);
        void yield();

    private://threads
        std::mutex _threadsMtx;
        typedef std::vector<worker::Thread *> TVThreads;
        TVThreads _threads;

    private://threads in wait
        std::mutex _threadsWaitMtx;
        TVThreads _threadsWait;

    private:
        worker::Effort      _effort;
    };
}}}

#endif
