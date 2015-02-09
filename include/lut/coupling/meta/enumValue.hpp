#pragma once

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    class EnumValue
        : public himpl::FaceLayout<impl::EnumValue, Type>
    {
    public:
        std::uint64_t numericalValue();

    };

}}}
