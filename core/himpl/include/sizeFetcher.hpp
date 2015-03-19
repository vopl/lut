#pragma once

#include <cstddef>

namespace dci { namespace himpl
{
    namespace details
    {
        template <class T>
        class EmptyProbe : T {char c;};
    }

    template <class T, bool isEmpty = (1 == sizeof(T) && 1 == sizeof(details::EmptyProbe<T>))>
    struct SizeFetcher
    {
        static const std::size_t _value = sizeof(T);
    };

    template <class T>
    struct SizeFetcher<T, true>
    {
        static const std::size_t _value = 0;
    };

}}
