#include <iostream>

#include "lut/async/stable.hpp"
#include "lut/async.hpp"
#include "lut/async/mutex.hpp"
#include "lut/async/event.hpp"
#include "lut/async/acquire.hpp"
#include "lut/mm.hpp"

#include <signal.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <cassert>
#include <vector>
#include <atomic>
#include <vector>
#include <iostream>
#include <cassert>
#include <atomic>
#include <cstdint>
#include <thread>
#include <functional>

std::atomic<std::size_t> cnt(0);

int lmain()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    lut::async::Mutex mtx1, mtx2;
    lut::async::Event evt1(true), evt2(true);


    auto f = [&](std::size_t num)
    {

        //atest<Allocator>(pallocator);

        char *c = (char *)alloca(4096);
        c[0] = 220;

        for(std::size_t i(0); i<100; i++)
        {
            if(1 == num)
            {
                evt2.set();
                evt1.acquire();
            }
            else
            {
                evt1.set();
                evt2.acquire();
            }

            std::cout<<num<<", pre yield"<<std::endl;
            lut::async::yield();
            std::cout<<num<<", post yield"<<std::endl;
        }

        if(mtx2.locked())
        {
            mtx2.unlock();
        }

        if(mtx1.locked())
        {
            mtx1.unlock();
        }

        std::cout<<num<<", "<<std::endl;


        cnt.fetch_add(-1, std::memory_order_relaxed);
    };

    lut::async::spawn(f, 1);
    lut::async::spawn(f, 2);


    for(std::size_t i(0);; i++)
    {
        lut::async::utilize();
    }

    return 0;
}

int main()
{
    lmain();
}
