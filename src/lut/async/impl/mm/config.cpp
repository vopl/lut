#include "lut/async/impl/mm/config.hpp"

namespace lut { namespace async { namespace impl { namespace mm
{

    const Config &Config::instance()
    {
        return _instance;
    }

    const Config Config::_instance;

}}}}
