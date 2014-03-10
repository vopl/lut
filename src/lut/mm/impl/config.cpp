#include "lut/mm/impl/config.hpp"

namespace lut { namespace mm { namespace impl
{

    const Config &Config::instance()
    {
        return _instance;
    }

    const Config Config::_instance;

}}}
