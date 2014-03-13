#include <iostream>

#include "lut/async/stable.hpp"
#include "lut/async/scheduler.hpp"
#include "lut/async/threadPool.hpp"

#include <signal.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <cassert>
#include <vector>
#include <atomic>

std::atomic<std::size_t> cnt(0);

int lmain()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    lut::async::Scheduler sched;

    lut::async::ThreadPool tp(sched, 1);


    auto f = [&](lut::async::Scheduler *sched, std::size_t iters){

        for(size_t i(0); i<200000; ++i)
        {
            void *p = lut::mm::Allocator::alloc<20>();
            //lut::mm::Allocator::free<20>(p);
        }

        char *c = (char *)alloca(4096);
        c[0] = 220;

        for(std::size_t i(0); i<iters; i++)
        {
            sched->yield();
        }

        cnt.fetch_add(-1, std::memory_order_relaxed);
    };

    for(std::size_t i(0); i<20; i++)
    {
        cnt.fetch_add(1);
        sched.spawn(f, &sched, 100000);
    }

    while(cnt.load(std::memory_order_relaxed))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

int main()
{
    lmain();
}
