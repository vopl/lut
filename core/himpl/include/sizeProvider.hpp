#pragma once

#include <cstddef>

namespace dci { namespace himpl
{
    template <class T>
    struct sizeProvider;
}}

#if GENERATE_SIZEPROVIDER
#   define SIZEPROVIDERFILE(x) <dci/himpl/sizeProviderDefault.hpp>
#else
#   define SIZEPROVIDERFILE(x) x
#endif
