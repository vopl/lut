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

    if(1)
    {
        sched.threadUtilize(std::chrono::nanoseconds(200));
        sched.threadUtilize(std::chrono::seconds(2));
        sched.threadUtilize(std::chrono::system_clock::now()+std::chrono::seconds(2));

        std::thread t(
                    [&t,&sched] ()
                    {
                        lut::async::ThreadUtilizationResult etur = sched.threadUtilize(std::chrono::nanoseconds(200));
                        (void)etur;
                        assert(lut::async::ThreadUtilizationResult::limitExhausted == etur);

                        assert(lut::async::ThreadReleaseResult::notInWork == sched.threadRelease());
                        assert(lut::async::ThreadReleaseResult::notInWork == sched.threadRelease(t.native_handle()));
                    });

        t.join();
        assert(lut::async::ThreadReleaseResult::notInWork == sched.threadRelease(t.native_handle()));
    }

    return 0;
}

int main()
{
    lmain();
}
