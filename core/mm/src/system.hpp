#pragma once

#include <cstddef>

namespace dci { namespace mm { namespace system
{

    void *malloc(std::size_t);
    void *calloc(std::size_t, std::size_t);
    void *realloc(void *, std::size_t);
    void free(void *);


}}}
