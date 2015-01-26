#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

namespace lut { namespace coupling { namespace meta
{
    struct Sign {};

    class Type
        : public hiddenImpl::FaceLayout<impl::Type>
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
