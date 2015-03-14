#include "lut/site/impl/instance.hpp"
#include "lut/site/error.hpp"
#include "lut/io/loop.hpp"
#include "lut/async/acquire.hpp"
#include "lut/async/functions.hpp"
#include "lut/logger/logger.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <cassert>
#include <tuple>
#include <iostream>

namespace fs = boost::filesystem;

namespace lut { namespace site { namespace impl
{
    Instance::Instance()
        : _modulesInitialized{false}
        , _modulesLoaded{false}
        , _modulesStarted{false}
        , _workState{WorkState::null}
    {
    }

    Instance::~Instance()
    {
    }

    std::error_code Instance::run()
    {
        switch(_workState)
        {
        case WorkState::null:
        case WorkState::stop:
            break;

        case WorkState::run:
            return make_error_code(error::general::already_runned);

        case WorkState::stopGraceful:
            return make_error_code(error::general::stopping_in_progress);

        default:
            assert("unknown work state");
            abort();
        }

        _workState = WorkState::run;
        async::spawn([this](){

            std::error_code ec = initializeModules().value<0>();
            if(ec)
            {
                LOGE("initialize modules: "<<ec.message());
                return;
            }

            ec = loadModules().value<0>();
            if(ec)
            {
                LOGE("load modules: "<<ec.message());
                return;
            }

            ec = startModules().value<0>();
            {
                LOGE("start modules: "<<ec.message());
                return;
            }
        });

        return lut::io::loop::run();
    }

    async::Future<std::error_code> Instance::stop(bool graceful)
    {
        switch(_workState)
        {
        case WorkState::null:
        case WorkState::stop:
            return async::mkReadyFuture(std::error_code{});

        case WorkState::run:
            _workState = WorkState::stopGraceful;
            {
                std::error_code ec = stopModules().value<0>();
                if(ec)
                {
                    LOGE("stop modules: "<<ec.message());
                    break;
                }

                ec = unloadModules().value<0>();
                if(ec)
                {
                    LOGE("unload modules: "<<ec.message());
                    break;
                }
            }

        case WorkState::stopGraceful:
            //secondary request - force hard stop
            break;

        default:
            assert("unknown work state");
            abort();
        }

        _workState = WorkState::stop;
        return async::mkReadyFuture(lut::io::loop::stop());
    }

    async::Future<std::error_code> Instance::initializeModules()
    {

        if(!_modulesInitialized)
        {
            _modulesInitialized = true;

            fs::path modulesDir = fs::current_path() / "modules";

            if(!fs::exists(modulesDir))
            {
                LOGE("site initialization: modules direactory is absent");
                return async::mkReadyFuture(make_error_code(error::general::modules_directory_absent));
            }

            fs::directory_iterator diter(modulesDir);
            fs::directory_iterator dend;
            for(; diter!=dend; ++diter)
            {
                if(fs::is_directory(*diter))
                {
                    module::Place place{diter->path().string()};
                    _modules.emplace_back(module::ControllerPtr{new module::Controller{place}});
                }
            }
        }

        return async::mkReadyFuture(std::error_code{});
    }

    async::Future<std::error_code> Instance::loadModules()
    {
        return massModulesOperation("load", [](const module::ControllerPtr &c)->async::Future<std::error_code> {
            return c->load();
        });
    }

    async::Future<std::error_code> Instance::startModules()
    {
        return massModulesOperation("start", [](const module::ControllerPtr &c)->async::Future<std::error_code> {
            return c->start();
        });
    }

    async::Future<std::error_code> Instance::stopModules()
    {
        return massModulesOperation("stop", [](const module::ControllerPtr &c)->async::Future<std::error_code> {
            return c->stop();
        });
    }

    async::Future<std::error_code> Instance::unloadModules()
    {
        return massModulesOperation("unload", [](const module::ControllerPtr &c)->async::Future<std::error_code> {
            return c->unload();
        });
    }

    template <class F>
    async::Future<std::error_code> Instance::massModulesOperation(const std::string &name, F operation)
    {
        bool hasErrors = false;

        std::vector<std::tuple<module::Controller*, async::Future<std::error_code>>> results;
        results.reserve(_modules.size());
        for(const auto &c : _modules)
        {
            results.emplace_back(std::make_tuple(c.get(), operation(c)));
        }

        for(auto &r : results)
        {
            if(std::get<1>(r).value<0>())
            {
                LOGE(name<<" module "<<std::get<0>(r)->getName()<<": "<<std::get<1>(r).value<0>().message());
                hasErrors = true;
            }
        }

        if(hasErrors)
        {
            async::mkReadyFuture(make_error_code(error::general::failed));
        }

        return async::mkReadyFuture(std::error_code{});
    }


}}}
