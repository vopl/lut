#include <iostream>

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
std::atomic<size_t> g_iters(1000);

std::atomic<size_t> g_amount(0);
std::size_t g_avgAmount(100);

void f(lut::async::Scheduler *sched)
{
    ++g_amount;

    if(g_iters > g_cnt)
    {
        if(g_amount < g_avgAmount)
        {
            sched->spawn(std::bind(f, sched));
        }
    }

    --g_amount;
    ++g_cnt;

    std::cout<<g_amount.load()<<std::endl;
}

int lmain()
{
    lut::async::Scheduler sched;

    lut::async::ThreadPool tp(sched, 2);

    sched.spawn(std::bind(f, &sched));

    while(g_cnt < g_iters || g_amount)
    {
        size_t cnt = g_cnt;
        size_t iters = g_iters;
        size_t amount = g_amount;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }


    return 0;
}

int main()
{
    lmain();
}
