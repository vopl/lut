#pragma once

#include "container1.hpp"
#include "container2.hpp"
#include <dci/himpl/implLayout.hpp>

namespace dci { namespace couple { namespace meta { namespace impl
{
    class Array
        : public himpl::ImplLayout<Array, Container1>
    {
    public:
        Array();
        ~Array();

        void setSize(std::uint32_t v);

    private:
        std::uint32_t _size{0};
    };

}}}}
