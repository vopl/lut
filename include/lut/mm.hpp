#ifndef _LUT_MM_HPP_
#define _LUT_MM_HPP_

#include "lut/mm/stack.hpp"

#include <type_traits>

namespace lut { namespace mm
{

    const Stack *stackAlloc();
    void stackFree(const Stack *stack);
    void stackCompact(const Stack *stack);

    const std::size_t maxAllocatedBufferSize = 1024;

    template <std::size_t size>
    void *alloc();

    template <std::size_t size>
    void free(void *ptr);


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
#define BAF_9238657287658752_0(N) \
    template <> void *alloc<N>(); \
    template <> void free<N>(void *ptr);

#define BAF_9238657287658752_1(N) \
    BAF_9238657287658752_0((N)*10 + 0) \
    BAF_9238657287658752_0((N)*10 + 1) \
    BAF_9238657287658752_0((N)*10 + 2) \
    BAF_9238657287658752_0((N)*10 + 3) \
    BAF_9238657287658752_0((N)*10 + 4) \
    BAF_9238657287658752_0((N)*10 + 5) \
    BAF_9238657287658752_0((N)*10 + 6) \
    BAF_9238657287658752_0((N)*10 + 7) \
    BAF_9238657287658752_0((N)*10 + 8) \
    BAF_9238657287658752_0((N)*10 + 9) \


#define BAF_9238657287658752_2(N) \
    BAF_9238657287658752_1((N)*10 + 0) \
    BAF_9238657287658752_1((N)*10 + 1) \
    BAF_9238657287658752_1((N)*10 + 2) \
    BAF_9238657287658752_1((N)*10 + 3) \
    BAF_9238657287658752_1((N)*10 + 4) \
    BAF_9238657287658752_1((N)*10 + 5) \
    BAF_9238657287658752_1((N)*10 + 6) \
    BAF_9238657287658752_1((N)*10 + 7) \
    BAF_9238657287658752_1((N)*10 + 8) \
    BAF_9238657287658752_1((N)*10 + 9) \


    BAF_9238657287658752_2(0)
    BAF_9238657287658752_2(1)
    BAF_9238657287658752_2(2)
    BAF_9238657287658752_2(3)
    BAF_9238657287658752_2(4)
    BAF_9238657287658752_2(5)
    BAF_9238657287658752_2(6)
    BAF_9238657287658752_2(7)
    BAF_9238657287658752_2(8)
    BAF_9238657287658752_2(9)

    BAF_9238657287658752_1(100)
    BAF_9238657287658752_1(101)

    BAF_9238657287658752_0(1020)
    BAF_9238657287658752_0(1021)
    BAF_9238657287658752_0(1022)
    BAF_9238657287658752_0(1023)
    BAF_9238657287658752_0(1024)

#undef BAF_9238657287658752_0
#undef BAF_9238657287658752_1
#undef BAF_9238657287658752_2

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size> void *alloc()
    {
        return ::malloc(size);
    }

    template <std::size_t size> void free(void *ptr)
    {
        return ::free(ptr);
    }

}}

#endif
