#pragma once

namespace lut { namespace module
{
    enum class State
    {
        null,
        installed,
        installedCorrupted,
        loaded,
        started,
    };
}}
