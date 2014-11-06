#include "lut/mm/system.hpp"

extern "C"
{
    void *__libc_malloc(size_t a);
    void *__libc_calloc(size_t a, size_t b);
    void *__libc_realloc(void *a, size_t b);
    void __libc_free(void *a);

    void *__libc_valloc(size_t a);
    void *__libc_memalign(size_t a, size_t b);
    void *__libc_pvalloc(size_t a);


}

namespace lut { namespace mm { namespace system
{

    void *malloc(size_t a)
    {
        return __libc_malloc(a);
    }

    void *calloc(size_t a, size_t b)
    {
        return __libc_calloc(a, b);
    }

    void *realloc(void *a, size_t b)
    {
        return __libc_realloc(a, b);
    }

    void free(void *a)
    {
        return __libc_free(a);
    }

}}}
