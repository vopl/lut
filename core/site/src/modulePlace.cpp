#include <modulePlace.hpp>
#include "impl/modulePlace.hpp"

namespace dci { namespace site
{
    ModulePlace::ModulePlace(const std::string &dir)
        : himpl::FaceLayout<impl::ModulePlace>(dir)
    {
    }

    ModulePlace::~ModulePlace()
    {
    }

    const std::string &ModulePlace::getDir() const
    {
        return impl().getDir();
    }

    void ModulePlace::setDir(const std::string &dir)
    {
        return impl().setDir(dir);
    }

}}
