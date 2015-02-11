#pragma once

#include "lut/coupling/meta/named.hpp"
#include "lut/coupling/meta/type.hpp"
#include "lut/coupling/meta/compound.hpp"

namespace lut { namespace coupling { namespace meta
{
    class Scope
        : public himpl::FaceLayout<impl::Scope, Named, Compound<Type>, Compound<Scope>>
    {

    };

}}}
