#pragma once

#include "functions.hpp"

#include <cassert>

namespace dci { namespace mm
{
    template <typename T>
    struct NewDelete
    {
        void* operator new(size_t sz)
        {
            assert(sz == sizeof(T));

            return ::dci::mm::alloc<sizeof(T)>();
        }

        void operator delete(void *ptr)
        {
            ::dci::mm::free<sizeof(T)>(ptr);
        }
    };

}}
