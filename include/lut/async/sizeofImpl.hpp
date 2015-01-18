#pragma once

#include "lut/hiddenImpl/sizeProvider.hpp"

namespace lut {namespace async {namespace impl {class Scheduler;}}}
namespace lut {namespace async {namespace impl {class Task;}}}
namespace lut {namespace async {namespace impl {class Mutex;}}}
namespace lut {namespace async {namespace impl {class Event;}}}

namespace lut { namespace hiddenImpl {
    template <> struct sizeProvider<::lut::async::impl::Scheduler> { static const std::size_t _value =  992;};
    template <> struct sizeProvider<::lut::async::impl::Task> { static const std::size_t _value =  32;};
    template <> struct sizeProvider<::lut::async::impl::Mutex> { static const std::size_t _value =  40;};
    template <> struct sizeProvider<::lut::async::impl::Event> { static const std::size_t _value =  40;};
}}

