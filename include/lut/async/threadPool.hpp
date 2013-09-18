#ifndef _ASYNC_THREADPOOL_HPP_
#define _ASYNC_THREADPOOL_HPP_

#include "async/threadUtilizer.hpp"

#include <vector>

namespace async
{

    class ThreadPool
    {
    public:
        ThreadPool(const async::ThreadUtilizer &tu, size_t amount = std::thread::hardware_concurrency());
        ~ThreadPool();

    private:
        async::ThreadUtilizer _tu;
        std::vector<std::thread> _threads;
        std::vector<ThreadState> _states;
    };
}

#endif
