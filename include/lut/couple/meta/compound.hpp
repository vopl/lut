#pragma once

#if !GENERATE_SIZEPROVIDER
#   include "lut/couple/meta/sizeProvider.hpp"
#endif

#include "lut/himpl/faceLayout.hpp"

namespace lut { namespace couple { namespace meta
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
