#pragma once

#include "type.hpp"

namespace dci { namespace couple { namespace meta
{
    enum class PrimitiveKind
    {
        void_,

        bool_,

        string,

        int8,
        int16,
        int32,
        int64,

        uint8,
        uint16,
        uint32,
        uint64,

        real32,
        real64,
    };

    class Primitive
        : public himpl::FaceLayout<impl::Primitive, Type>
    {
    public:
        PrimitiveKind kind();

    };

}}}
