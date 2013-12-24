#include <iostream>

#include "lut/async/stable.hpp"
#include "lut/async/scheduler.hpp"
#include "lut/async/threadPool.hpp"
#include "lut/async/event.hpp"
#include "lut/async/mutex.hpp"
#include "lut/async/wait.hpp"

#include <signal.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <cassert>
#include <vector>
#include <atomic>

std::atomic<size_t> cnt(0);

int lmain()
{

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    lut::async::Scheduler sched;

    lut::async::ThreadPool tp(sched, 4);


    auto f = [&](lut::async::Scheduler *sched, size_t iters){

        char *c = (char *)alloca(4096);
        c[0] = 220;

        for(size_t i(0); i<iters; i++)
        {
            sched->yield();
        }

        cnt.fetch_add(-1, std::memory_order_relaxed);
    };

    for(size_t i(0); i<20; i++)
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
