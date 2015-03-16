#include "lut/module/impl/place.hpp"

namespace lut { namespace module { namespace impl
{
    Place::Place()
        : _dir{}
    {
    }

    Place::Place(const std::string &dir)
        : _dir{dir}
    {
    }

    Place::~Place()
    {
    }

    const std::string &Place::getDir() const
    {
        return _dir;
    }

    void Place::setDir(const std::string &dir)
    {
        _dir = dir;
    }

}}}
