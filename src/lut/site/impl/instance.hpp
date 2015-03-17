#pragma once

#include <system_error>
#include "lut/site/impl/module.hpp"

namespace lut { namespace site { namespace impl
{
    enum class WorkState
    {
        stopped,
        starting,
        started,
        stopping,
    };

    class Instance
    {
    public:
        Instance();
        ~Instance();

        std::error_code run();
        async::Future<std::error_code> stop();

    private:
        std::error_code initializeModules();
        async::Future<std::error_code> loadModules();
        async::Future<std::error_code> startModules();
        async::Future<std::error_code> stopModules();
        async::Future<std::error_code> unloadModules();
        std::error_code deinitializeModules();

        template <class F>
        async::Future<std::error_code> massModulesOperation(const std::string &name, F operation);

    private:
        bool        _modulesInitialized;
        bool        _modulesLoaded;
        bool        _modulesStarted;
        WorkState   _workState;

    private:
        //TODO: состояние по проинсталированным модулям
        std::vector<ModulePtr> _modules;

    };
}}}
