#pragma once

#include <functions.hpp>

#include "allocator.hpp"
#include "system.hpp"
#include "allocator.ipp"
#include "utils/sized_cast.ipp"
#include "utils/intrusiveDeque.ipp"
#include "allocator/blocksHolder.ipp"
#include "allocator/block.ipp"
#include "allocator/bitIndex.ipp"
#include "allocator/bitIndex/level.ipp"

namespace dci { namespace mm
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
