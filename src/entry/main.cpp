#include <iostream>

#include "async/scheduler.hpp"
#include "async/threadUtilizer.hpp"
#include "async/codeManager.hpp"
#include "async/threadPool.hpp"
#include "async/event.hpp"
#include "async/mutex.hpp"
#include "async/wait.hpp"
#include "async/impl/coro.hpp"

#include <iostream>
#include <thread>
#include <cassert>
#include <vector>
#include <atomic>

int lmain()
{
    async::Scheduler sched;
    async::ThreadUtilizer tu(sched);
    async::CodeManager cm(sched);

    if(0)
    {
        tu.te_utilize(std::chrono::nanoseconds(200));
        tu.te_utilize(std::chrono::minutes(200));
        tu.te_utilize(std::chrono::system_clock::now()+std::chrono::minutes(200));

        std::thread t(
                    [&t,&tu] ()
                    {
                        async::EThreadUtilizationResult etur = tu.te_utilize(std::chrono::nanoseconds(200));
                        (void)etur;
                        assert(async::etur_limitExhausted == etur);

                        assert(async::etrr_notInWork == tu.te_release());
                        assert(async::etrr_notInWork == tu.release(t.native_handle()));
                    });

        t.join();
        assert(async::etrr_notInWork == tu.release(t.native_handle()));
    }


    try
    {
        async::ThreadPool tp(tu, 4);

        std::atomic<size_t> cnt(0);
        size_t amount = 1000;
        size_t mult = 300*100;
        async::Event event0(true);
        char pad0[64];
        async::Event event1(true);
        char pad1[64];
        async::Event event2(true);
        char pad2[64];
        async::Event event3(true);
        char pad3[64];

        event0.set();
        event1.set();
        event2.set();
        event3.set();

#define TXTMAKE 0
#define TXTOUT 0
        for(size_t i(0); i<amount; i++)
        {
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            ////std::this_thread::yield();

            cm.spawn([i, mult, &cnt, &event0, &event1, &event2, &event3]{

                for(size_t m(0); m<mult; m++)
                {
                    size_t k = i*mult+m;
                    if(0 == (k%4))
                    {
                        uint32_t mtxIndex = async::waitAny(event0, event1, event2, event3);
                        int v = cnt.fetch_add(1);
                        (void)v;
#if TXTMAKE
                        char tmp[32];
                        (void)tmp;
                        sprintf(tmp, "%d, in %d locked %d\n", v, (int)(k%4), mtxIndex);
#if TXTOUT
                        std::cout<<tmp; std::cout.flush();
#endif
#endif
                        switch(mtxIndex)
                        {
                        case 0:
                            event0.set();
                            break;
                        case 1:
                            event1.set();
                            break;
                        case 2:
                            event2.set();
                            break;
                        case 3:
                            event3.set();
                            break;
                        default:
                            assert(0);
                        }
                    }
                    else if(1 == (k%4))
                    {
                        uint32_t mtxIndex = async::waitAny(event0, event1, event2, event3);
                        int v = cnt.fetch_add(1);
                        (void)v;
#if TXTMAKE
                        char tmp[32];
                        (void)tmp;
                        sprintf(tmp, "%d, in %d locked %d\n", v, (int)(k%4), mtxIndex);
#if TXTOUT
                        std::cout<<tmp; std::cout.flush();
#endif
#endif
                        switch(mtxIndex)
                        {
                        case 0:
                            event0.set();
                            break;
                        case 1:
                            event1.set();
                            break;
                        case 2:
                            event2.set();
                            break;
                        case 3:
                            event3.set();
                            break;
                        default:
                            assert(0);
                        }
                    }
                    else if(2 == (k%4))
                    {
                        uint32_t mtxIndex = async::waitAny(event0, event1, event2, event3);
                        int v = cnt.fetch_add(1);
                        (void)v;
#if TXTMAKE
                        char tmp[32];
                        (void)tmp;
                        sprintf(tmp, "%d, in %d locked %d\n", v, (int)(k%4), mtxIndex);
#if TXTOUT
                        std::cout<<tmp; std::cout.flush();
#endif
#endif
                        switch(mtxIndex)
                        {
                        case 0:
                            event0.set();
                            break;
                        case 1:
                            event1.set();
                            break;
                        case 2:
                            event2.set();
                            break;
                        case 3:
                            event3.set();
                            break;
                        default:
                            assert(0);
                        }
                    }
                    else if(3 == (k%4))
                    {
                        uint32_t mtxIndex = async::waitAny(event0, event1, event2, event3);
                        int v = cnt.fetch_add(1);
                        (void)v;
#if TXTMAKE
                        char tmp[32];
                        (void)tmp;
                        sprintf(tmp, "%d, in %d locked %d\n", v, (int)(k%4), mtxIndex);
#if TXTOUT
                        std::cout<<tmp; std::cout.flush();
#endif
#endif
                        switch(mtxIndex)
                        {
                        case 0:
                            event0.set();
                            break;
                        case 1:
                            event1.set();
                            break;
                        case 2:
                            event2.set();
                            break;
                        case 3:
                            event3.set();
                            break;
                        default:
                            assert(0);
                        }
                    }
                }
            });
        }

        while(cnt < amount*mult)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }

        std::cout<<"done "<<cnt<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    catch(...)
    {
        std::cout<<"exception"<<std::endl;
    }

    return 0;
}

int main()
{
    lmain();
}
