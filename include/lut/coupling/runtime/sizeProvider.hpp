#pragma once

#include "lut/himpl/sizeProvider.hpp"

namespace lut {namespace coupling {namespace runtime {namespace impl {class Hub;}}}}
namespace lut {namespace coupling {namespace runtime {namespace impl {class Iface;}}}}

namespace lut { namespace himpl {
    template <> struct sizeProvider<::lut::coupling::runtime::impl::Hub> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::runtime::impl::Iface> { static const std::size_t _value =  0;};
}}

