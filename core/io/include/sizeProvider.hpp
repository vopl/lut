#pragma once

#include <dci/himpl/sizeProvider.hpp>

namespace dci {namespace io {namespace impl {class Stream;}}}
namespace dci {namespace io {namespace impl {class Client;}}}
namespace dci {namespace io {namespace impl {class Server;}}}
namespace dci {namespace io {namespace impl {class Data;}}}

namespace dci { namespace himpl {
    template <> struct sizeProvider<::dci::io::impl::Stream> { static const std::size_t _value =  16;};
    template <> struct sizeProvider<::dci::io::impl::Client> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::dci::io::impl::Server> { static const std::size_t _value =  56;};
    template <> struct sizeProvider<::dci::io::impl::Data> { static const std::size_t _value =  24;};
}}

