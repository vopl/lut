#pragma once

#include <dci/himpl/implLayout.hpp>

namespace dci { namespace site { namespace impl
{
    class ModulePlace
        : public himpl::ImplLayout<ModulePlace>
    {
    public:
        ModulePlace();
        ModulePlace(const std::string &dir);
        ~ModulePlace();

        const std::string &getDir() const;
        void setDir(const std::string &dir);

    private:
        std::string _dir;
    };
}}}
