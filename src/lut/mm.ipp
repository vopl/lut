#pragma once

#include "lut/mm.hpp"
#include "lut/mm/allocator.hpp"
#include "lut/mm/system.hpp"
#include "lut/mm/allocator.ipp"
#include "lut/mm/utils/sized_cast.ipp"
#include "lut/mm/utils/intrusiveDeque.ipp"
#include "lut/mm/allocator/blocksHolder.ipp"
#include "lut/mm/allocator/block.ipp"
#include "lut/mm/allocator/bitIndex.ipp"
#include "lut/mm/allocator/bitIndex/level.ipp"

namespace lut { namespace mm
{
    namespace details
    {
        template <std::size_t sizeClass>
        inline void *allocBySizeClass(std::size_t size)
        {
            (void)size;
            return g_allocator.alloc<sizeClass>();
        }

        template <std::size_t sizeClass>
        inline void freeBySizeClass(void *ptr)
        {
            g_allocator.free<sizeClass>(ptr);
        }

        template <>
        inline void *allocBySizeClass<ConfigHeap::_bigSizeClass>(std::size_t size)
        {
            return system::malloc(size);
        }

        template <>
        inline void freeBySizeClass<ConfigHeap::_bigSizeClass>(void *ptr)
        {
            system::free(ptr);
        }
    }

    inline void *alloc(std::size_t size)
    {
        return g_allocator.alloc(size);
    }

    inline void free(void *ptr)
    {
        return g_allocator.free(ptr);
    }

    inline std::size_t size(void *ptr)
    {
        return g_allocator.size(ptr);
    }

}}
