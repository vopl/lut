#pragma once

#include "lut/himpl/sizeProvider.hpp"

namespace lut {namespace site {namespace impl {class ModulePlace;}}}

namespace lut { namespace himpl {
    template <> struct sizeProvider<::lut::site::impl::ModulePlace> { static const std::size_t _value =  8;};
}}

