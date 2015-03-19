#pragma once

namespace dci { namespace site { namespace impl
{
    enum class ModuleState
    {
        null,

        attached,
        attachError,

        loading,
        loaded,
        loadError,
        unloading,

        starting,
        started,
        startError,
        stopping,
    };
}}}
