#pragma once

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")

#include <dci/himpl/faceLayout.hpp>

#include <vector>
#include <string>

namespace dci { namespace couple { namespace meta
{
    template <class E>
    class Compound
        : public himpl::FaceLayout<impl::Compound<E>>
    {
    public:
        std::vector<E *> elements();

        std::size_t elementsAmount();

        E *element(std::size_t index);
        E *element(std::string name);

    };

}}}
