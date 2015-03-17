#pragma once

#include "lut/himpl/faceLayout.hpp"

#if !GENERATE_SIZEPROVIDER
#   include "lut/site/sizeProvider.hpp"
#endif

#include <string>

namespace lut { namespace site
{
    namespace impl
    {
        class ModulePlace;
    }

    class ModulePlace
        : public himpl::FaceLayout<impl::ModulePlace>
    {

    public:
        ModulePlace(const std::string &dir);
        ~ModulePlace();

        const std::string &getDir() const;
        void setDir(const std::string &dir);
    };
}}
