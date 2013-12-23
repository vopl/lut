#ifndef _LUT_ASYNC_IMPL_SM_VM_HPP_
#define _LUT_ASYNC_IMPL_SM_VM_HPP_

#include <cstddef>

namespace lut { namespace async { namespace impl { namespace sm { namespace vm
{
    struct User
    {
        virtual bool vmHandleAccess(void *addr) = 0;
    };

    bool startup(User *user);
    bool shutdown(User *user);

    void *alloc(size_t size);
    bool free(void *addr, size_t size);

    bool protect(void *addr, size_t size, bool access);
}}}}}

#endif
