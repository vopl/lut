#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

namespace lut { namespace coupling { namespace meta
{
    struct Sign {};

    class Type
        : public hiddenImpl::Layout<impl::Type>
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
