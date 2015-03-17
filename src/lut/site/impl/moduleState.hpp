#pragma once

namespace lut { namespace site { namespace impl
{
    enum class State
    {
        null,
        installed,
        installedCorrupted,
        loaded,
        started,
    };
}}}
