#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/coupling/meta/sizeProvider.hpp"
#endif
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace coupling { namespace meta
{
    struct Sign {};

    class Type
        : public himpl::FaceLayout<impl::Type>
    {
    public:

        //есть незаданные-неразрешенные части, рекурсивно
        bool complete();

        //уникальная метка этого типа
        Sign sign();

        //расположение
        Type *parent();

        //имя
        std::string name();

    };

}}}
