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
            return lut::mm::alloc<sizeof(pod)>();
        }

        void NOINLINE free(void *p)
        {
            lut::mm::free<sizeof(pod)>(p);
        }
    };
}


namespace
{

#define MAXAMOUNT 2500
            using pvoid = void *;
            pvoid ps[MAXAMOUNT];

    template <class Allocator>
    size_t atest(Allocator *pa)
    {
        std::cout<<"begin"<<std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        size_t res(0);
        for(size_t t(0); t<100000; t++)
        {
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
