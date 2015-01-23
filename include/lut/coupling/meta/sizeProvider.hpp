#pragma once

#include "lut/hiddenImpl/sizeProvider.hpp"

#include "lut/coupling/meta/predecl.hpp"

namespace lut { namespace hiddenImpl {
    template <> struct sizeProvider<::lut::coupling::meta::impl::Alias> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Array> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Attribute> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Compound<::lut::coupling::meta::Attribute>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Compound<::lut::coupling::meta::EnumValue>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Compound<::lut::coupling::meta::Method>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Compound<::lut::coupling::meta::Type>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Container1> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Container2> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Enum> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::EnumValue> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Iface> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Enum>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Iface>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Struct>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Inheritable<::lut::coupling::meta::Variant>> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::List> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Map> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Method> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Ptr> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Primitive> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Scope> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Set> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Struct> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Type> { static const std::size_t _value =  0;};
    template <> struct sizeProvider<::lut::coupling::meta::impl::Variant> { static const std::size_t _value =  0;};
}}

