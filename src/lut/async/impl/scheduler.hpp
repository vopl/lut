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
    class Thread;

    class Scheduler
    {
    public:
        Scheduler();
        ~Scheduler();

        ThreadReleaseResult releaseThread(const std::thread::id &id);

    public:
        bool threadEntry_init(Thread *thread);
        bool threadEntry_deinit(Thread *thread);

    public:
        void spawn(Task *code);
        void yield();

    private://threads
        std::mutex _threadsMtx;
        typedef std::vector<worker::Thread *> TVThreads;
        TVThreads _threads;

    private:
        worker::Effort      _effort;
    };
}}}

#endif
