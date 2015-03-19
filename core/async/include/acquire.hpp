#pragma once

#include <cstdint>

namespace dci { namespace async
{
    template <typename... Acquirable>
    std::size_t acquireAny(Acquirable&... acquirables);

    template <typename... Acquirable>
    void acquireAll(Acquirable&... acquirables);
}}

#include "dci/async/details/acquire.hpp"

namespace dci { namespace async
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
