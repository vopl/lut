#ifndef _LUT_ASYNC_IMPL_SM_VM_HPP_
#define _LUT_ASYNC_IMPL_SM_VM_HPP_

#include <cstddef>

namespace lut { namespace async { namespace impl { namespace sm { namespace vm
{
    typedef bool (*TVmAccessHandler)(void *addr);

    bool threadInit(TVmAccessHandler accessHandler);
    bool threadDeinit(TVmAccessHandler accessHandler);

    void *alloc(std::size_t size);
    bool free(void *addr, std::size_t size);

    bool protect(void *addr, std::size_t size, bool access);
}}}}}

#endif
