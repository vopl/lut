#pragma once

#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/compound.hpp"
#include "lut/coupling/meta/enumValue.hpp"
#include "lut/coupling/meta/inheritable.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Enum
        : public himpl::FaceLayout<impl::Enum, Type, Compound<EnumValue>, Inheritable<Enum>>
    {
    public:
        std::vector<EnumValue *> values();

        std::uint32_t bitness();


    };

}}}
