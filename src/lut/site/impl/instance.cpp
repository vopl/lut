#include "lut/site/impl/instance.hpp"
#include "lut/io/loop.hpp"
#include <cassert>

namespace lut { namespace site { namespace impl
{
    Instance::Instance(Mode mode)
        : _mode(mode)
        , _stopMode(StopMode::nonStop)
    {

    }

    Instance::~Instance()
    {

    }

    std::error_code Instance::run()
    {
        return lut::io::loop::run();
    }

    std::error_code Instance::stop(StopMode stopMode)
    {
        switch(stopMode)
        {
        case StopMode::nonStop:
            _stopMode = StopMode::nonStop;
            return std::error_code{};

        case StopMode::graceful:
            if(StopMode::nonStop == _stopMode)
            {
                _stopMode = StopMode::graceful;
                //assert(!"not impl");
                //abort();
                return std::error_code{};
            }
            return lut::io::loop::stop();

        case StopMode::immediately:
            return lut::io::loop::stop();
        }

        assert(!"unknown stop mode");
        return lut::io::loop::stop();
    }

}}}
