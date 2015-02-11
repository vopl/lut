#include "lut/stable.hpp"
#include "lut/coupling/meta/impl/named.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
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
