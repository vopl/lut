#pragma once

#include "lut/hiddenImpl/sizeProvider.hpp"

namespace lut {namespace coupling {namespace meta {namespace impl {class Array;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Enum;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Iface;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Integer;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class List;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Map;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Ptr;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Real;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Scope;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Set;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class String;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Struct;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Type;}}}}
namespace lut {namespace coupling {namespace meta {namespace impl {class Variant;}}}}

namespace lut { namespace hiddenImpl {
    template <> struct sizeProvider<::lut::coupling::meta::impl::Array> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Enum> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Iface> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Integer> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::List> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Map> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Ptr> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Real> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Scope> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Set> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::String> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Struct> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Type> { static const std::size_t _value =  1;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Variant> { static const std::size_t _value =  1;};
}}

