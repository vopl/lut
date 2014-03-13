#ifndef _LUT_MM_ALLOCATOR_HPP_
#define _LUT_MM_ALLOCATOR_HPP_

#include "lut/mm/stack.hpp"

#include <type_traits>

namespace lut { namespace mm
{

    struct Allocator
    {

    public:
        static bool threadInit();
        static bool threadDeinit();

    public:
        static const Stack *stackAlloc();
        static void stackFree(const Stack *stack);
        static void stackCompact(const Stack *stack);

    public:
        template <std::size_t size, std::size_t align = alignof(typename std::aligned_storage<size>::type)>
        static void *alloc();

        template <std::size_t size, std::size_t align = alignof(typename std::aligned_storage<size>::type)>
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
#define BAF_9238657287658752(N0) \
    template <> void *Allocator::allocAligned<N0>(); \
    template <> void Allocator::freeAligned<N0>(void *ptr);

#define BAF_9238658994592102(N1) \
    BAF_9238657287658752(N1*10 + 1) \
    BAF_9238657287658752(N1*10 + 2) \
    BAF_9238657287658752(N1*10 + 3) \
    BAF_9238657287658752(N1*10 + 4) \
    BAF_9238657287658752(N1*10 + 5) \
    BAF_9238657287658752(N1*10 + 6) \
    BAF_9238657287658752(N1*10 + 7) \
    BAF_9238657287658752(N1*10 + 8) \
    BAF_9238657287658752(N1*10 + 9) \
    BAF_9238657287658752(N1*10 + 10) \

    BAF_9238658994592102(0)
    BAF_9238658994592102(1)
    BAF_9238658994592102(2)
    BAF_9238658994592102(3)
    BAF_9238658994592102(4)
    BAF_9238658994592102(5)
    BAF_9238658994592102(6)
    BAF_9238658994592102(7)
    BAF_9238658994592102(8)
    BAF_9238658994592102(9)
    BAF_9238658994592102(10)
    BAF_9238658994592102(11)
    BAF_9238658994592102(12)
    BAF_9238658994592102(13)
    BAF_9238658994592102(14)
    BAF_9238658994592102(15)
    BAF_9238658994592102(16)
    BAF_9238658994592102(17)
    BAF_9238658994592102(18)
    BAF_9238658994592102(19)
    BAF_9238658994592102(20)
    BAF_9238658994592102(21)
    BAF_9238658994592102(22)
    BAF_9238658994592102(23)
    BAF_9238658994592102(24)
    BAF_9238658994592102(25)
    BAF_9238658994592102(26)
    BAF_9238658994592102(27)
    BAF_9238658994592102(28)
    BAF_9238658994592102(29)
    BAF_9238658994592102(30)
    BAF_9238658994592102(31)
    BAF_9238658994592102(32)
    BAF_9238658994592102(33)
    BAF_9238658994592102(34)
    BAF_9238658994592102(35)
    BAF_9238658994592102(36)
    BAF_9238658994592102(37)
    BAF_9238658994592102(38)
    BAF_9238658994592102(39)
    BAF_9238658994592102(40)
    BAF_9238658994592102(41)
    BAF_9238658994592102(42)
    BAF_9238658994592102(43)
    BAF_9238658994592102(44)
    BAF_9238658994592102(45)
    BAF_9238658994592102(46)
    BAF_9238658994592102(47)
    BAF_9238658994592102(48)
    BAF_9238658994592102(49)
    BAF_9238658994592102(50)

    BAF_9238657287658752(511)
    BAF_9238657287658752(512)

#undef BAF_9238657287658752
#undef BAF_9238658994592102

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
