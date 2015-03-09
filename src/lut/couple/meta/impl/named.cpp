#include "lut/stable.hpp"
#include "lut/couple/meta/impl/named.hpp"

namespace lut { namespace couple { namespace meta { namespace impl
{
    Named::Named()
    {

    }

    Named::~Named()
    {

    }

    void Named::setName(const std::string &name)
    {
        _name = name;
    }

}}}}
