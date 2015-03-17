#include "lut/site/impl/modulePlace.hpp"

namespace lut { namespace site { namespace impl
{
    ModulePlace::ModulePlace()
        : _dir{}
    {
    }

    ModulePlace::ModulePlace(const std::string &dir)
        : _dir{dir}
    {
    }

    ModulePlace::~ModulePlace()
    {
    }

    const std::string &ModulePlace::getDir() const
    {
        return _dir;
    }

    void ModulePlace::setDir(const std::string &dir)
    {
        _dir = dir;
    }

}}}
