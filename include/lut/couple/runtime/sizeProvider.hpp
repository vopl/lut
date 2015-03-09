#pragma once

#include "lut/himpl/sizeProvider.hpp"

namespace lut {namespace couple {namespace runtime {namespace impl {class Hub;}}}}
namespace lut {namespace couple {namespace runtime {namespace impl {class Iface;}}}}

namespace lut { namespace himpl {
    template <> struct sizeProvider<::lut::couple::runtime::impl::Hub> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::couple::runtime::impl::Iface> { static const std::size_t _value =  0;};
}}

