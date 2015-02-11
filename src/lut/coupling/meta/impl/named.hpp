#pragma once

#include "lut/himpl/implLayout.hpp"

#include <string>

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Named
        : public himpl::ImplLayout<Named>
    {

    public:
        Named();
        ~Named();

        void setName(const std::string &name);

    private:
        std::string _name;
    };

}}}}
