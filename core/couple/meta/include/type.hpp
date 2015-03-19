#pragma once

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")
#include "predecl.hpp"
#include <dci/himpl/faceLayout.hpp>

namespace dci { namespace couple { namespace meta
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

    };

}}}
