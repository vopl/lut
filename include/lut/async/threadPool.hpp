#ifndef _LUT_ASYNC_THREADPOOL_HPP_
#define _LUT_ASYNC_THREADPOOL_HPP_

#include "lut/async/scheduler.hpp"

#include <thread>
#include <vector>

namespace lut { namespace async
{

    class ThreadPool
    {
    public:
        ThreadPool(Scheduler &scheduler, size_t amount = std::thread::hardware_concurrency());
        ~ThreadPool();

    private:
        Scheduler                   &_scheduler;
        std::vector<std::thread>    _threads;
        std::vector<ThreadState>    _states;
    };
}}

#endif
