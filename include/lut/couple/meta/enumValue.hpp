#pragma once

#include "lut/couple/meta/named.hpp"

namespace lut { namespace couple { namespace meta
{
    class EnumValue
        : public himpl::FaceLayout<impl::EnumValue, Named>
    {
    public:
        std::uint64_t numericalValue();

    };

}}}
