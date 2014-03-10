#ifndef _LUT_MM_IMPL_VM_HPP_
#define _LUT_MM_IMPL_VM_HPP_

#include <cstddef>

namespace lut { namespace mm { namespace impl { namespace vm
{
    typedef bool (*TVmAccessHandler)(void *addr);

    bool threadInit(TVmAccessHandler accessHandler);
    bool threadDeinit(TVmAccessHandler accessHandler);

    void *alloc(size_t size);
    bool free(void *addr, size_t size);

    bool protect(void *addr, size_t size, bool access);
}}}}

#endif
