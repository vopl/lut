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
#include <vector>
#include <iostream>
#include <cassert>
#include <atomic>
#include <cstdint>
#include <thread>
#include <functional>
#include <ext/mt_allocator.h>
#include "../dreams/segregated_storage/FSBAllocator.hh"
#include "../dreams/segregated_storage/pool_c.hpp"
#include "../dreams/segregated_storage/pool_c_st.hpp"

#include <boost/pool/pool.hpp>

std::atomic<std::size_t> cnt(0);

struct pod
{
    int i;
    int j;
    int k;
    int l;
//    char _data[16];
//    char _data2[32];
//    char _data3[64];
};

namespace
{
//#define NOINLINE __attribute__ ((noinline))
#define NOINLINE

    struct LocalAllocator
    {
        using Target = test::Pool_c<sizeof(pod), 1024>;
        Target _p;

        void *NOINLINE alloc()
        {
            return _p.alloc();
        }

        void NOINLINE free(void *p)
        {
            return _p.free(p);
        }
    };

    struct OriginalAllocator
    {
        test_st::Pool_c _p;

        OriginalAllocator()
        {
            _p.CreatePool(sizeof(pod), 65536);

        }

        void *NOINLINE alloc()
        {
            return _p.Allocate();
        }

        void NOINLINE free(void *p)
        {
            return _p.DeAllocate(p);
        }
    };

    struct MAllocator
    {
        void *NOINLINE alloc()
        {
            return ::malloc(sizeof(pod));
        }

        void NOINLINE free(void *p)
        {
            return ::free(p);
        }
    };

    struct FSAllocator
    {
        FSBAllocator<pod> _a;
        void *NOINLINE alloc()
        {
            return _a.allocate(1);
        }

        void NOINLINE free(void *p)
        {
            return _a.deallocate((pod*)p, sizeof(pod));
        }
    };

    struct FSAllocator2
    {
        FSBAllocator2<pod> _a;
        void *NOINLINE alloc()
        {
            return _a.allocate(1);
        }

        void NOINLINE free(void *p)
        {
            return _a.deallocate((pod*)p, sizeof(pod));
        }
    };


    struct BPAllocator
    {
        boost::pool<> _p;

        BPAllocator()
            : _p(sizeof(pod), 65536)
        {

        }

        void *NOINLINE alloc()
        {
            return _p.malloc();
        }

        void NOINLINE free(void *p)
        {
            _p.free(p);
        }
    };

    struct Local2Allocator
    {
        Local2Allocator()
        {

        }

        void *NOINLINE alloc()
        {
            return lut::mm::Allocator::alloc<sizeof(pod)>();
        }

        void NOINLINE free(void *p)
        {
            lut::mm::Allocator::free<sizeof(pod)>(p);
        }
    };
}


namespace
{

    template <class Allocator>
    size_t atest(Allocator *pa)
    {
        std::cout<<"begin"<<std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        size_t res(0);
        for(size_t t(0); t<1000000; t++)
        {
#define MAXAMOUNT 250
            using pvoid = void *;
            pvoid ps[MAXAMOUNT];

            size_t amount = rand()%MAXAMOUNT;

            for(size_t i(0); i<amount; i++)
            {
                ps[i] = pa->alloc();
            }

            for(size_t i(0); i<amount; i++)
            {
                pod *pp = (pod *)ps[i];
                pp->i = 0x1;
//                pp->j = 0x2;
//                pp->k = 0x3;
//                pp->l = 0x4;
            }

            for(size_t i(amount-1); i<amount; i--)
            {
                pa->free(ps[i]);
            }
            res += amount;
        }
        std::cout<<"end "<<res<<std::endl;

        return res;
    }
}

//using Allocator = OriginalAllocator;
//using Allocator = LocalAllocator;
//using Allocator = MAllocator;
//using Allocator = FSAllocator;
//using Allocator = FSAllocator2;
//using Allocator = BPAllocator;
using Allocator = Local2Allocator;

Allocator *pallocator = new Allocator;

int lmain()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    lut::async::Scheduler sched;

    lut::async::ThreadPool tp(sched, 1);


    auto f = [&](lut::async::Scheduler *sched, std::size_t iters){

        atest<Allocator>(pallocator);
        exit(0);

        char *c = (char *)alloca(4096);
        c[0] = 220;

        for(std::size_t i(0); i<iters; i++)
        {
            sched->yield();
        }

        cnt.fetch_add(-1, std::memory_order_relaxed);
    };

    for(std::size_t i(0); i<1; i++)
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
