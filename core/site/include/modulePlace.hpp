#pragma once

#include <dci/himpl/faceLayout.hpp>

#include <dci/himpl/sizeProvider.hpp>
#include SIZEPROVIDERFILE("sizeProvider.hpp")

#include <string>

namespace dci { namespace site
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
