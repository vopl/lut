#pragma once

#include "lut/himpl/sizeProvider.hpp"

#include "lut/couple/meta/predecl.hpp"

namespace lut { namespace himpl {
    template <> struct sizeProvider<::lut::couple::meta::impl::Alias> { static const std::size_t _value =  16;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Array> { static const std::size_t _value =  16;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Attribute> { static const std::size_t _value =  16;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Compound<::lut::couple::meta::Attribute>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Compound<::lut::couple::meta::EnumValue>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Compound<::lut::couple::meta::Method>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Compound<::lut::couple::meta::Type>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Compound<::lut::couple::meta::Scope>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Container1> { static const std::size_t _value =  8;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Container2> { static const std::size_t _value =  16;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Enum> { static const std::size_t _value =  56;};
    template <> struct sizeProvider<::lut::couple::meta::impl::EnumValue> { static const std::size_t _value =  8;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Iface> { static const std::size_t _value =  104;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Enum>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Iface>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Struct>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Inheritable<::lut::couple::meta::Variant>> { static const std::size_t _value =  24;};
    template <> struct sizeProvider<::lut::couple::meta::impl::List> { static const std::size_t _value =  8;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Map> { static const std::size_t _value =  16;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Method> { static const std::size_t _value =  48;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Ptr> { static const std::size_t _value =  8;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Primitive> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Scope> { static const std::size_t _value =  56;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Set> { static const std::size_t _value =  8;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Struct> { static const std::size_t _value =  104;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Named> { static const std::size_t _value =  8;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Type> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Variant> { static const std::size_t _value =  104;};
    template <> struct sizeProvider<::lut::couple::meta::impl::Library> { static const std::size_t _value =  792;};
    template <> struct sizeProvider<::lut::couple::meta::impl::LibraryBuilder> { static const std::size_t _value =  792;};
}}

