#pragma once

#include <dci/himpl/sizeProvider.hpp>

namespace dci {namespace async {namespace impl {class Scheduler;}}}
namespace dci {namespace async {namespace impl {class Task;}}}
namespace dci {namespace async {namespace impl {class Mutex;}}}
namespace dci {namespace async {namespace impl {class Event;}}}

namespace dci { namespace himpl {
    template <> struct sizeProvider<::dci::async::impl::Scheduler> { static const std::size_t _value =  992;};
    template <> struct sizeProvider<::dci::async::impl::Task> { static const std::size_t _value =  32;};
    template <> struct sizeProvider<::dci::async::impl::Mutex> { static const std::size_t _value =  40;};
    template <> struct sizeProvider<::dci::async::impl::Event> { static const std::size_t _value =  40;};
}}

