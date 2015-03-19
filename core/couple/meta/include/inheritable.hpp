#pragma once

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")

#include <dci/himpl/faceLayout.hpp>

namespace dci { namespace couple { namespace meta
{
    template <class B>
    class Inheritable
        : public himpl::FaceLayout<impl::Inheritable<B>>
    {
    public:
        std::vector<B *> bases();


    };

}}}
