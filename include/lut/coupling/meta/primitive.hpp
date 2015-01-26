#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

#include "lut/coupling/meta/type.hpp"

namespace lut { namespace coupling { namespace meta
{
    enum PrimitiveKind
    {
        void_,

        bool_,

        string,

        sint8,
        sint16,
        sint32,
        sint64,

        uint8,
        uint16,
        uint32,
        uint64,

        real32,
        real64,
    };

    class Primitive
        : public hiddenImpl::FaceLayout<impl::Primitive, Type>
    {
    public:
        PrimitiveKind kind();

    };

}}}
