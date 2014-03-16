#include "lut/mm/stable.hpp"
#include "lut/mm/allocator.hpp"
#include "lut/mm/impl/allocator.hpp"

namespace lut { namespace mm
{

    bool Allocator::threadInit()
    {
        return impl::Allocator::instance().threadInit();
    }

    bool Allocator::threadDeinit()
    {
        return impl::Allocator::instance().threadDeinit();
    }

    const Stack *Allocator::stackAlloc()
    {
        return impl::Allocator::instance().stackAlloc();
    }

    void Allocator::stackFree(const Stack *stack)
    {
        return impl::Allocator::instance().stackFree(stack);
    }

    void Allocator::stackCompact(const Stack *stack)
    {
        return impl::Allocator::instance().stackCompact(stack);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
#define BAF_9238657287658752(N0) \
    template <> void *Allocator::allocAligned<N0>() \
    { \
        return impl::Allocator::instance().alloc<N0>(); \
    } \
    template <> void Allocator::freeAligned<N0>(void *ptr) \
    { \
        return impl::Allocator::instance().free<N0>(ptr); \
    } \

#define BAF_9238658994592102(N1) \
    BAF_9238657287658752(N1*10 + 0) \
    BAF_9238657287658752(N1*10 + 1) \
    BAF_9238657287658752(N1*10 + 2) \
    BAF_9238657287658752(N1*10 + 3) \
    BAF_9238657287658752(N1*10 + 4) \
    BAF_9238657287658752(N1*10 + 5) \
    BAF_9238657287658752(N1*10 + 6) \
    BAF_9238657287658752(N1*10 + 7) \
    BAF_9238657287658752(N1*10 + 8) \
    BAF_9238657287658752(N1*10 + 9) \

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

    BAF_9238657287658752(510)
    BAF_9238657287658752(511)
    BAF_9238657287658752(512)

#undef BAF_9238657287658752
#undef BAF_9238658994592102

}}
