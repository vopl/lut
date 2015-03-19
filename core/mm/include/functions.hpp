#pragma once

#include "config.hpp"
#include "stack.hpp"

namespace dci { namespace mm
{
    namespace details
    {
        ////////////////////////////////////////////////////////////////
        template <std::size_t sizeClass> void *allocBySizeClass(std::size_t size);
        template <std::size_t sizeClass> void freeBySizeClass(void *ptr);
    }

    ////////////////////////////////////////////////////////////////
    template <std::size_t size> void *alloc()
    {
        return details::allocBySizeClass<ConfigHeap::evalSizeClass(size)>(size);
    }

    template <std::size_t size> void free(void *ptr)
    {
        return details::freeBySizeClass<ConfigHeap::evalSizeClass(size)>(ptr);
    }

    ////////////////////////////////////////////////////////////////
    void *alloc(std::size_t size);
    void free(void *ptr);
    std::size_t size(void *ptr);











    const Stack *stackAlloc();
    void stackFree(const Stack *stack);
    void stackCompact(const Stack *stack);

}}
