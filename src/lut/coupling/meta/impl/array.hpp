#pragma once

#include "lut/coupling/meta/impl/container1.hpp"
#include "lut/coupling/meta/impl/container2.hpp"
#include "lut/himpl/implLayout.hpp"

namespace lut { namespace coupling { namespace meta { namespace impl
{
    class Array
        : public himpl::ImplLayout<Container1>
    {
    public:
        Array();
        ~Array();

        void setSize(std::uint32_t v);
    };

}}}}
