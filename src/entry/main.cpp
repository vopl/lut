#include <iostream>

#include "lut/async/stable.hpp"
#include "lut/async/scheduler.hpp"
#include "lut/async/threadPool.hpp"
#include "lut/async/event.hpp"
#include "lut/async/mutex.hpp"
#include "lut/async/wait.hpp"

#include <iostream>
#include <thread>
#include <cassert>
#include <vector>
#include <atomic>

std::atomic<size_t> g_cnt(0);
std::atomic<size_t> g_iters(1000000);

std::atomic<size_t> g_amount(0);
std::size_t g_avgAmount(100);

void f(lut::async::Scheduler *sched)
{
    ++g_cnt;

    if(! (g_cnt % 10000))
    {
        std::cout<<g_amount.load()<<", "<<g_cnt<<std::endl;
    }

//    for(size_t k(0); k<10; k++)
//    {
//        sched->yield();
//    }

//    sched->yield();
    if(g_iters > g_cnt)
    {
        if(g_amount < g_avgAmount)
        {
            ++g_amount;
            sched->spawn(&f, sched);
            sched->yield();
        }
        if(g_amount < g_avgAmount)
        {
            ++g_amount;
            sched->spawn(&f, sched);
            sched->yield();
        }
    }

    --g_amount;
}

int lmain()
{
    lut::async::Scheduler sched;

    lut::async::ThreadPool tp(sched, 4);

    ++g_amount;
    sched.spawn(&f, &sched);

    while(g_cnt < g_iters || g_amount)
    {
        size_t cnt = g_cnt;
        (void)cnt;
        size_t iters = g_iters;
        (void)iters;
        size_t amount = g_amount;
        (void)amount;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }


    return 0;
}

int main()
{
    lmain();
}
