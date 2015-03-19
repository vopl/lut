#pragma once

#include <dci/himpl/implLayout.hpp>

#include <string>

namespace dci { namespace couple { namespace meta { namespace impl
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
