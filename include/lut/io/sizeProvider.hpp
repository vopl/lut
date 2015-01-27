#pragma once

#include "lut/himpl/sizeProvider.hpp"

namespace lut {namespace io {namespace impl {class Stream;}}}
namespace lut {namespace io {namespace impl {class Client;}}}
namespace lut {namespace io {namespace impl {class Server;}}}
namespace lut {namespace io {namespace impl {class Data;}}}

namespace lut { namespace himpl {
    template <> struct sizeProvider<::lut::io::impl::Stream> { static const std::size_t _value =  16;};
    template <> struct sizeProvider<::lut::io::impl::Client> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::io::impl::Server> { static const std::size_t _value =  56;};
    template <> struct sizeProvider<::lut::io::impl::Data> { static const std::size_t _value =  24;};
}}

