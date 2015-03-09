#pragma once

#include "lut/couple/meta/type.hpp"
#include "lut/couple/meta/named.hpp"
#include "lut/couple/meta/compound.hpp"
#include "lut/couple/meta/enumValue.hpp"
#include "lut/couple/meta/inheritable.hpp"

namespace lut { namespace couple { namespace meta
{
    class Enum
        : public himpl::FaceLayout<impl::Enum, Type, Named, Compound<EnumValue>, Inheritable<Enum>>
    {
    public:
        std::vector<EnumValue *> values();

        std::uint32_t bitness();


    };

}}}
