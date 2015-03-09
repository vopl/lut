#pragma once

#include "lut/couple/meta/container1.hpp"

namespace lut { namespace couple { namespace meta
{
    class Array
        : public himpl::FaceLayout<impl::Array, Container1>
    {
    public:
        std::uint32_t size();

    };

}}}
