#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/layout.hpp"

namespace lut { namespace coupling { namespace meta
{
    template <class B>
    class Inheritable
        : public hiddenImpl::Layout<impl::Inheritable<B>>
    {
    public:
        std::vector<B *> bases();


    };

}}}
