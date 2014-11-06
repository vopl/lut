#pragma once

#include "lut/mm.hpp"
#include "lut/mm/allocator.hpp"
#include "lut/mm/system.hpp"

namespace lut { namespace mm
{
    namespace details
    {
        template <std::size_t sizeClass>
        inline void *allocBySizeClass()
        {
            return g_allocator.alloc<sizeClass>();
        }

        template <std::size_t sizeClass>
        inline void freeBySizeClass(void *ptr)
        {
            g_allocator.free<sizeClass>(ptr);
        }

        template <>
        inline void *allocBySizeClass<ConfigHeap::_bigSizeClass>()
        {
            return system::malloc(ConfigHeap::_bigSizeClass);
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
