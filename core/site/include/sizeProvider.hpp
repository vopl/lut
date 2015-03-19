#pragma once

#include <dci/himpl/sizeProvider.hpp>

namespace dci {namespace site {namespace impl {class ModulePlace;}}}

namespace dci { namespace himpl {
    template <> struct sizeProvider<::dci::site::impl::ModulePlace> { static const std::size_t _value =  8;};
}}

