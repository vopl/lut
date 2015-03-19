#pragma once

#include "named.hpp"

namespace dci { namespace couple { namespace meta
{
    class EnumValue
        : public himpl::FaceLayout<impl::EnumValue, Named>
    {
    public:
        std::uint64_t numericalValue();

    };

}}}
