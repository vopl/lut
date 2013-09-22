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

int lmain()
{
    lut::async::Scheduler sched;

    lut::async::ThreadState threadStateEvt;
    std::thread t(
                [&t,&sched, &threadStateEvt] ()
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(10));
                    lut::async::ThreadUtilizationResult etur = sched.threadUtilize(&threadStateEvt);
                    (void)etur;
                    assert(lut::async::ThreadUtilizationResult::releaseRequest == etur);

                    assert(lut::async::ThreadReleaseResult::notInWork == sched.threadRelease());
                    assert(lut::async::ThreadReleaseResult::notInWork == sched.threadRelease(t.native_handle()));
                });

    sched.spawn([&sched](){
        std::cout<<"in coro"<<std::endl;

        sched.spawn([](){
            std::cout<<"in coro2"<<std::endl;


        });

    });

    threadStateEvt.wait(lut::async::ThreadStateValue::inWork);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sched.threadRelease(t.native_handle());
    t.join();

    return 0;
}

int main()
{
    lmain();
}
