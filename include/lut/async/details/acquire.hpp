#ifndef _LUT_ASYNC_DETAILS_ACQUIRE_HPP_
#define _LUT_ASYNC_DETAILS_ACQUIRE_HPP_

#include <cstdint>

namespace lut { namespace async { namespace details
{
    template <typename... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables)
    {
        assert(0);
    }

    template <typename... Acquirable>
    void acquireAll(Acquirable&... acquirables)
    {
        assert(0);
    }
}}}

#endif
