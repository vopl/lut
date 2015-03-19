#pragma once

#include "named.hpp"
#include "type.hpp"
#include "compound.hpp"

namespace dci { namespace couple { namespace meta
{
    class Scope
        : public himpl::FaceLayout<impl::Scope, Named, Compound<Type>, Compound<Scope>>
    {

    };

}}}
