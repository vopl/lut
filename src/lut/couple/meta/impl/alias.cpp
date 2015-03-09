#include "lut/stable.hpp"
#include "lut/couple/meta/impl/alias.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    Alias::Alias()
    {

    }

    Alias::~Alias()
    {

    }

    void Alias::setTarget(Type *v)
    {
        _target = v;
    }

}}}}
