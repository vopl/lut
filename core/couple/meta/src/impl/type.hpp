#pragma once

#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Type
        : public himpl::ImplLayout<Type>
    {

    public:
        Type();
        ~Type();
    };

}}}}
