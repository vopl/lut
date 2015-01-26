#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/hiddenImpl/faceLayout.hpp"

namespace lut { namespace coupling { namespace meta
{
    template <class B>
    class Inheritable
        : public hiddenImpl::FaceLayout<impl::Inheritable<B>>
    {
    public:
        std::vector<B *> bases();


    };

}}}
