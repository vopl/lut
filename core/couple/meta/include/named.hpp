#pragma once

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")
#include <dci/himpl/faceLayout.hpp>

#include <string>

namespace dci { namespace couple { namespace meta
{
    class Named
        : public himpl::FaceLayout<impl::Named>
    {
    public:
        //имя
        std::string name();

    };

}}}
