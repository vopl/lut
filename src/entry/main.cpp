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

    std::thread t(
                [&t,&sched] ()
                {
                    lut::async::ThreadUtilizationResult etur = sched.threadUtilize();
                    (void)etur;
                    assert(lut::async::ThreadUtilizationResult::limitExhausted == etur);

                    assert(lut::async::ThreadReleaseResult::notInWork == sched.threadRelease());
                    assert(lut::async::ThreadReleaseResult::notInWork == sched.threadRelease(t.native_handle()));
                });

    sched.spawn([](){
        std::cout<<"in coro"<<std::endl;
    });

    sched.threadRelease(t.native_handle());
    t.join();

    return 0;
}

int main()
{
    lmain();
}
