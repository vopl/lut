#pragma once

#include "lut/mm/functions.hpp"

namespace lut { namespace mm
{
    template <typename T>
    struct NewDelete
    {
        void* operator new(size_t sz)
        {
            assert(sz == sizeof(T));

            return ::lut::mm::alloc<sizeof(T)>();
        }

        void operator delete(void *ptr)
        {
            ::lut::mm::free<sizeof(T)>(ptr);
        }
    };

}}
