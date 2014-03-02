#include <vector>
#include <iostream>
#include <cassert>
#include <atomic>
#include <cstdint>
#include <thread>
#include <functional>
#include <ext/mt_allocator.h>

#include "pool_c.hpp"
#include "pool_c_st.hpp"
#include "FSBAllocator.hh"

#include <boost/pool/pool.hpp>



struct pod
{
//    int i;
//    int j;
//    int k;
//    int l;
    char _data[12];
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

//            for(size_t i(0); i<amount; i++)
//            {
//                pod *pp = (pod *)ps[i];
//                pp->i = 0x1;
////                pp->j = 0x2;
////                pp->k = 0x3;
////                pp->l = 0x4;
//            }

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


int main()
{

    //using Allocator = OriginalAllocator;
    using Allocator = LocalAllocator;
    //using Allocator = MAllocator;
    //using Allocator = FSAllocator;
    //using Allocator = FSAllocator2;
    //using Allocator = BPAllocator;
    Allocator *pallocator = new Allocator;


    std::vector<std::thread> threads;


    for(size_t i(0); i<1; i++) {
        threads.emplace_back(std::thread(&atest<Allocator>, pallocator));
    }

    for(size_t i(0); i<threads.size(); i++) {
        threads[i].join();
    }

  return 0;
}
