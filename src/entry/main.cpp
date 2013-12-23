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

void ctlc(int sig, siginfo_t * info, void * context)
{
    printf("in handler\n");
    exit(0);
}

int lmain()
{

    std::thread t([&]{

//        struct sigaction act;
//        memset(&act, 0, sizeof(act));
//        act.sa_flags = SA_SIGINFO;
//        act.sa_sigaction = ctlc;
//        sigaction(SIGSEGV, &act, NULL);

        printf("in thread\n");

        *(int *)0x12345 = 220;
    });

    t.join();

    return 0;
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    lut::async::Scheduler sched;

    lut::async::ThreadPool tp(sched, 1);


    auto f = [&](lut::async::Scheduler *sched, size_t iters){

        struct sigaction act;
        memset(&act, 0, sizeof(act));
        act.sa_flags = SA_SIGINFO;
        act.sa_sigaction = ctlc;
        sigaction(SIGSEGV, &act, NULL);

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
