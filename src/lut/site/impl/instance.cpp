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
        : _modulesLoaded{false}
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
            std::error_code ec = loadModules().value<0>();
            if(ec)
            {
                LOGE("load modules: "<<ec.message());
            }
            else
            {
                ec = startModules().value<0>();
                {
                    LOGE("start modules: "<<ec.message());
                }
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

    async::Future<std::error_code> Instance::loadModules()
    {
        fs::path modulesDir = fs::current_path() / "modules";

        if(!fs::exists(modulesDir))
        {
            return async::mkReadyFuture(make_error_code(error::general::modules_directory_absent));
        }

        return async::mkReadyFuture(std::error_code{});
    }

    async::Future<std::error_code> Instance::startModules()
    {
        assert(0);
    }

    async::Future<std::error_code> Instance::stopModules()
    {
        bool hasErrors = false;

        std::vector<std::tuple<module::Controller*, async::Future<std::error_code>>> results;
        results.reserve(_modules.size());
        for(const auto &c : _modules)
        {
            results.emplace_back(std::make_tuple(c.get(), c->stop()));
        }

        for(auto &r : results)
        {
            if(std::get<1>(r).value<0>())
            {
                LOGE("stop module "<<std::get<0>(r)->getName()<<": "<<std::get<1>(r).value<0>().message());
                hasErrors = true;
            }
        }

        if(hasErrors)
        {
            async::mkReadyFuture(make_error_code(error::general::failed));
        }

        return async::mkReadyFuture(std::error_code{});
    }

    async::Future<std::error_code> Instance::unloadModules()
    {
        assert(0);
    }

}}}
