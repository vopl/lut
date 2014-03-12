#ifndef _LUT_MM_IMPL_VM_HPP_
#define _LUT_MM_IMPL_VM_HPP_

#include <cstddef>

namespace lut { namespace mm { namespace impl { namespace vm
{
    typedef bool (*TVmAccessHandler)(void *addr);

    bool threadInit(TVmAccessHandler accessHandler);
    bool threadDeinit(TVmAccessHandler accessHandler);

    void *alloc(std::size_t size);
    bool free(const void *addr, std::size_t size);

    bool protect(const void *addr, std::size_t size, bool access);
}}}}

#endif
