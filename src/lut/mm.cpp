#include "lut/stable.hpp"
#include "lut/mm.hpp"
#include "lut/mm/system.hpp"
#include "lut/mm/allocator.hpp"

#include "lut/mm.ipp"
#include "lut/mm/allocator.ipp"
#include "lut/mm/allocator/blocksHolder.ipp"
#include "lut/mm/allocator/block.ipp"
#include "lut/mm/utils/intrusiveDeque.ipp"
#include "lut/mm/utils/sized_cast.ipp"
#include "lut/mm/allocator/bitIndex.ipp"
#include "lut/mm/allocator/bitIndex/level.ipp"

namespace lut { namespace mm
{
    const Stack *stackAlloc()
    {
        return g_allocator.stackAlloc();
    }

    void stackFree(const Stack *stack)
    {
        return g_allocator.stackFree(stack);
    }

    void stackCompact(const Stack *stack)
    {
        return g_allocator.stackCompact(stack);
    }

    namespace
    {
        /////////////////////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass>
        std::uintptr_t bySizeClassInstantiate()
        {
            return
                bySizeClassInstantiate<ConfigHeap::evalPrevSizeClass(sizeClass-1)>() +
                reinterpret_cast<std::uintptr_t>(&details::allocBySizeClass<sizeClass>) +
                reinterpret_cast<std::uintptr_t>(&details::freeBySizeClass<sizeClass>);
        }

        /////////////////////////////////////////////////////////////////////////////////
        template <>
        std::uintptr_t bySizeClassInstantiate<ConfigHeap::_minSize>()
        {
            return
                reinterpret_cast<std::uintptr_t>(&details::allocBySizeClass<ConfigHeap::_minSize>) +
                reinterpret_cast<std::uintptr_t>(&details::freeBySizeClass<ConfigHeap::_minSize>);
        }

        static const volatile std::uintptr_t bySizeClassInstantiateSideEffect = bySizeClassInstantiate<ConfigHeap::_maxSize>();
    }

}}
