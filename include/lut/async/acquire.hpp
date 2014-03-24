#ifndef _LUT_ASYNC_ACQUIRE_HPP_
#define _LUT_ASYNC_ACQUIRE_HPP_

#include <cstdint>

namespace lut { namespace async
{
    template <typename... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables);

    template <typename... Acquirable>
    void acquireAll(Acquirable&... acquirables);
}}

#include "lut/async/details/acquire.hpp"

namespace lut { namespace async
{
    template <typename... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables)
    {
        return details::acquireAny(acquirables...);
    }

    template <typename... Acquirable>
    void acquireAll(Acquirable&... acquirables)
    {
        return details::acquireAll(acquirables...);
    }
}}

#endif
