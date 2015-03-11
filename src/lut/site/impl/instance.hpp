#pragma once

#include <system_error>
#include "lut/module/controller.hpp"

namespace lut { namespace site { namespace impl
{
    enum class Mode
    {
        Master,
        Slave
    };

    enum class StopMode
    {
        nonStop,
        immediately,
        graceful
    };

    class Instance
    {
    public:
        Instance(Mode mode);
        ~Instance();

        std::error_code run();
        std::error_code stop(StopMode stopMode);

    private:
        Mode        _mode;
        StopMode    _stopMode;

    private:

        std::vector<module::ControllerPtr> _controllers;

    };
}}}
