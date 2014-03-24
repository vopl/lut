#ifndef _LUT_MM_ALLOCATOR_HPP_
#define _LUT_MM_ALLOCATOR_HPP_

#include "lut/mm/stack.hpp"

#include <type_traits>

namespace lut { namespace mm
{

    struct Allocator
    {
    public:
        static const Stack *stackAlloc();
        static void stackFree(const Stack *stack);
        static void stackCompact(const Stack *stack);

    public:
        static const std::size_t _maxAllocatedBufferSize = 1024;

    public:
        template <std::size_t size, std::size_t align = 1>
        static void *alloc();

        template <std::size_t size, std::size_t align = 1>
        static void free(void *ptr);

    private:
        template <std::size_t size>
        static void *allocAligned();

        template <std::size_t size>
        static void freeAligned(void *ptr);
    };


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size, std::size_t align>
    void *Allocator::alloc()
    {
        return allocAligned<sizeof(typename std::aligned_storage<size, align>::type)>();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <std::size_t size, std::size_t align>
    void Allocator::free(void *ptr)
    {
        return freeAligned<sizeof(typename std::aligned_storage<size, align>::type)>(ptr);
    }


    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
#define BAF_9238657287658752_0(N) \
    template <> void *Allocator::allocAligned<N>(); \
    template <> void Allocator::freeAligned<N>(void *ptr);

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
    template <std::size_t size> void *Allocator::allocAligned()
    {
        return ::malloc(size);
    }

    template <std::size_t size> void Allocator::freeAligned(void *ptr)
    {
        return ::free(ptr);
    }

}}

#endif
