#include "lut/module/place.hpp"
#include "lut/module/impl/place.hpp"

namespace lut { namespace module
{
    Place::Place(const std::string &dir)
        : himpl::FaceLayout<impl::Place>(dir)
    {
    }

    Place::~Place()
    {
    }

    const std::string &Place::getDir() const
    {
        return impl().getDir();
    }

    void Place::setDir(const std::string &dir)
    {
        return impl().setDir(dir);
    }

}}
