#pragma once

#include "lut/hiddenImpl/sizeofImpl.hpp"

namespace lut {namespace io {namespace impl {class Handle;}}}

namespace lut { namespace hiddenImpl {
    template <> struct sizeofImpl<::lut::io::impl::Handle> { static const std::size_t _value =  1;};
}}

