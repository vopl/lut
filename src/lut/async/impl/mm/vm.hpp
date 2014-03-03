#ifndef _LUT_ASYNC_IMPL_MM_VM_HPP_
#define _LUT_ASYNC_IMPL_MM_VM_HPP_

#include <cstddef>

namespace lut { namespace async { namespace impl { namespace mm { namespace vm
{
    typedef bool (*TVmAccessHandler)(void *addr);

    bool threadInit(TVmAccessHandler accessHandler);
    bool threadDeinit(TVmAccessHandler accessHandler);

    void *alloc(size_t size);
    bool free(void *addr, size_t size);

    bool protect(void *addr, size_t size, bool access);
}}}}}

#endif
