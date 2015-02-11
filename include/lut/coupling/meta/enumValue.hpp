#pragma once

#include "lut/coupling/meta/named.hpp"

namespace lut { namespace coupling { namespace meta
{
    class EnumValue
        : public himpl::FaceLayout<impl::EnumValue, Named>
    {
    public:
        std::uint64_t numericalValue();

    };

}}}
