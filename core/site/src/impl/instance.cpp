#include "instance.hpp"
#include <error.hpp>
#include <dci/io/loop.hpp>
#include <dci/async/acquire.hpp>
#include <dci/async/functions.hpp>
#include <dci/logger/logger.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <cassert>
#include <tuple>
#include <iostream>

namespace fs = boost::filesystem;

namespace dci { namespace site { namespace impl
{
    Instance::Instance()
        : _modulesInitialized{false}
        , _modulesLoaded{false}
        , _modulesStarted{false}
        , _workState{WorkState::stopped}
    {
    }

    Instance::~Instance()
    {
    }

    std::error_code Instance::run()
    {
        switch(_workState)
        {
        case WorkState::stopped:
            break;

        case WorkState::starting:
            return make_error_code(error::general::starting_in_progress);

        case WorkState::started:
            return make_error_code(error::general::already_started);

        case WorkState::stopping:
            return make_error_code(error::general::stopping_in_progress);

        default:
            assert("unknown work state");
            abort();
        }

        _workState = WorkState::starting;
        async::spawn([this](){

            std::error_code ec = initializeModules();
            if(ec)
            {
                LOGE("initialize modules: "<<ec);
            }

            ec = loadModules().value<0>();
            if(ec)
            {
                LOGE("load modules: "<<ec);
            }

            ec = startModules().value<0>();
            if(ec)
            {
                LOGE("start modules: "<<ec);
            }

            _workState = WorkState::started;
        });

        return dci::io::loop::run();
    }

    async::Future<std::error_code> Instance::stop()
    {
        switch(_workState)
        {
        case WorkState::started:
            break;

        case WorkState::stopped:
            return async::mkReadyFuture(make_error_code(error::general::already_stopped));

        case WorkState::starting:
            return async::mkReadyFuture(make_error_code(error::general::starting_in_progress));

        case WorkState::stopping:
            return async::mkReadyFuture(make_error_code(error::general::stopping_in_progress));

        default:
            assert("unknown work state");
            abort();
        }

        async::Promise<std::error_code> p;
        async::Future<std::error_code> f = p.future();

        _workState = WorkState::stopping;
        async::spawn([p=std::move(p), this]() mutable{

            bool hasErrors = false;

            std::error_code ec = stopModules().value<0>();
            if(ec)
            {
                LOGE("stop modules: "<<ec);
                hasErrors = true;
            }

            ec = unloadModules().value<0>();
            if(ec)
            {
                LOGE("unload modules: "<<ec);
                hasErrors = true;
            }

            ec = deinitializeModules();
            if(ec)
            {
                LOGE("detach modules: "<<ec);
                hasErrors = true;
            }


            _workState = WorkState::stopped;

            ec = io::loop::stop();
            if(ec)
            {
                LOGE("stop io loop: "<<ec);
                hasErrors = true;
            }

            p.setValue(
                        hasErrors ?
                            make_error_code(error::general::parial_failed) :
                            std::error_code{});
        });

        return f;
    }

    std::error_code Instance::initializeModules()
    {
        if(!_modulesInitialized)
        {
            _modulesInitialized = true;

            fs::path modulesDir = fs::current_path() / "modules";

            if(!fs::exists(modulesDir))
            {
                LOGE("site initialization: modules direactory is absent");
                return make_error_code(error::general::modules_directory_absent);
            }

            bool hasErrors = false;

            fs::directory_iterator diter(modulesDir);
            fs::directory_iterator dend;
            for(; diter!=dend; ++diter)
            {
                if(fs::is_directory(*diter))
                {
                    ModulePtr c{new Module};
                    std::error_code ec = c->attach(diter->path().string());
                    if(ec)
                    {
                        LOGE("unable attach module in "<<diter->path().string()<<": "<<ec);
                        hasErrors = true;
                    }
                    else
                    {
                        _modules.emplace_back(std::move(c));
                    }
                }
            }

            return hasErrors ?
                        make_error_code(error::general::parial_failed) :
                        std::error_code{};
        }

        return std::error_code{};
    }

    async::Future<std::error_code> Instance::loadModules()
    {
        if(!_modulesLoaded)
        {
            _modulesLoaded = true;

            return massModulesOperation("load", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->load();
            });
        }

        return async::mkReadyFuture(std::error_code{});
    }

    async::Future<std::error_code> Instance::startModules()
    {
        if(!_modulesStarted)
        {
            _modulesStarted = true;

            return massModulesOperation("start", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->start();
            });
        }

        return async::mkReadyFuture(std::error_code{});
    }

    async::Future<std::error_code> Instance::stopModules()
    {
        if(_modulesStarted)
        {
            _modulesStarted = false;

            return massModulesOperation("stop", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->stop();
            });
        }

        return async::mkReadyFuture(std::error_code{});
    }

    async::Future<std::error_code> Instance::unloadModules()
    {
        if(_modulesLoaded)
        {
            _modulesLoaded = false;

            return massModulesOperation("unload", [](const ModulePtr &c)->async::Future<std::error_code> {
                return c->unload();
            });
        }

        return async::mkReadyFuture(std::error_code{});
    }

    std::error_code Instance::deinitializeModules()
    {
        if(_modulesInitialized)
        {
            _modulesInitialized = false;

            bool hasErrors = false;

            for(const auto &c : _modules)
            {
                std::error_code ec = c->detach();
                if(ec)
                {
                    LOGE("unable detach module \""<<c->getName()<<"\": "<<ec);
                    hasErrors = true;
                }
            }

            _modules.clear();

            return hasErrors ?
                        make_error_code(error::general::parial_failed) :
                        std::error_code{};
        }

        return std::error_code{};
    }

    template <class F>
    async::Future<std::error_code> Instance::massModulesOperation(const std::string &name, F operation)
    {
        async::Promise<std::error_code> p;
        async::Future<std::error_code> f = p.future();

        async::spawn([p=std::move(p), this, name, operation]() mutable {

            bool hasErrors = false;

            std::vector<std::tuple<Module*, async::Future<std::error_code>>> results;
            results.reserve(_modules.size());
            for(const auto &c : _modules)
            {
                results.emplace_back(std::make_tuple(c.get(), operation(c)));
            }

            for(auto &r : results)
            {
                if(std::get<1>(r).value<0>())
                {
                    LOGE(name<<" module \""<<std::get<0>(r)->getName()<<"\": "<<std::get<1>(r).value<0>());
                    hasErrors = true;
                }
            }

            p.setValue(
                        hasErrors ?
                            make_error_code(error::general::parial_failed) :
                            std::error_code{});
        });

        return f;
    }


}}}
