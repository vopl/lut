#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/container1.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Array
        : public hiddenImpl::FaceLayout<impl::Array, Container1>
    {
    public:
        std::uint64_t size();

    };

}}}
