#pragma once

#include "lut/coupling/meta/sizeProvider.hpp"
#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace coupling { namespace meta
{
    template <class B>
    class Inheritable
        : public himpl::FaceLayout<impl::Inheritable<B>>
    {
    public:
        std::vector<B *> bases();


    };

}}}
