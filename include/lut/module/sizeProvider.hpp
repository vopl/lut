#pragma once

#include "lut/himpl/sizeProvider.hpp"

namespace lut {namespace module {namespace impl {class Controller;}}}
namespace lut {namespace module {namespace impl {class Place;}}}

namespace lut { namespace himpl {
    template <> struct sizeProvider<::lut::module::impl::Controller> { static const std::size_t _value =  232;};
    template <> struct sizeProvider<::lut::module::impl::Place> { static const std::size_t _value =  8;};
}}

