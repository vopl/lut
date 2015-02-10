#pragma once

#include "lut/himpl/implLayout.hpp"

#include <string>

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Type
        : public himpl::ImplLayout<>
    {

    public:
        Type();
        ~Type();

        void setName(const std::string &name);

    private:
        std::string _name;
    };

}}}}
