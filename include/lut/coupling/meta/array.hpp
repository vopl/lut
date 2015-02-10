#pragma once

#include "lut/coupling/meta/container1.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Array
        : public himpl::FaceLayout<impl::Array, Container1>
    {
    public:
        std::uint32_t size();

    };

}}}
