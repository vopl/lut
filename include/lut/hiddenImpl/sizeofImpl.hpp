#ifndef _LUT_HIDDENIMPL_SIZEOFIMPL_HPP_
#define _LUT_HIDDENIMPL_SIZEOFIMPL_HPP_

#include <cstddef>

namespace lut { namespace hiddenImpl
{
    template <class T>
    struct sizeofImpl
    {
        static const std::size_t _value = 1;
    };
}}

#endif
