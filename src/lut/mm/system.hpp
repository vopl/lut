#pragma once
#include <cstddef>

namespace lut { namespace mm { namespace system
{

    void *malloc(size_t);
    void *calloc(size_t, size_t);
    void *realloc(void *, size_t);
    void free(void *);


}}}
