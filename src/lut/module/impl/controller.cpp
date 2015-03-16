#include "lut/module/impl/controller.hpp"
#include "lut/module/error.hpp"
#include "lut/logger/logger.hpp"
#include "lut/async/functions.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>

#include <dlfcn.h>

namespace lut { namespace module { namespace impl
{
    Controller::Controller()
        : himpl::ImplLayout<Controller>{}
        , _provider{}
        , _id{}
        , _serviceIds{}

        , _revision{}
        , _name{}
        , _description{}
        , _tags{}

        , _requiredServiceIds{}
        , _requiredModuleIds{}

        , _mainBinary{}

        , _state{State::null}
        , _place{}

        , _moduleInstall{}
        , _moduleUninstall{}

        , _moduleLoad{}
        , _moduleUnload{}

        , _moduleStart{}
        , _moduleStop{}

        , _moduleGetServiceInstance{}

    {
    }

    Controller::~Controller()
    {
        assert(State::null == _state);
    }

    std::error_code Controller::attach(const Place &place)
    {
        if(State::null != _state)
        {
            return make_error_code(error::general::module_already_attached);
        }
        _place = place;
        _state = State::installedCorrupted;

        boost::property_tree::ptree pt;

        boost::filesystem::path path(_place.getDir());
        path /= "index.json";

        try
        {
            boost::property_tree::read_json(path.string(), pt);
        }
        catch(std::runtime_error &e)
        {
            LOGE("unable to parse module info: "<<e.what());
            return make_error_code(error::general::bad_info_file);
        }


        try
        {
            _provider = pt.get<std::string>("provider");
            if(!_id.fromHex(pt.get<std::string>("id")))
            {
               throw std::runtime_error("malformed id: "+pt.get<std::string>("id"));
            }

            _serviceIds.clear();
            for(auto &v: pt.get_child("serviceIds"))
            {
                _serviceIds.push_back(couple::runtime::Iid{});
                if(!_serviceIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed service id: "+v.second.data());
                }
            }

            _revision = pt.get<std::size_t>("revision");
            _name = pt.get<std::string>("name");
            _description = pt.get<std::string>("description");

            _tags.clear();
            for(auto &v: pt.get_child("tags"))
            {
                _tags.emplace_back(v.second.data());
            }

            _requiredServiceIds.clear();
            for(auto &v: pt.get_child("requiredServiceIds"))
            {
                _requiredServiceIds.push_back(couple::runtime::Iid{});
                if(!_requiredServiceIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed required service id: "+v.second.data());
                }
            }

            _requiredModuleIds.clear();
            for(auto &v: pt.get_child("requiredModuleIds"))
            {
                _requiredModuleIds.push_back(Mid{});
                if(!_requiredModuleIds.back().fromHex(v.second.data()))
                {
                   throw std::runtime_error("malformed required module id: "+v.second.data());
                }
            }

            _mainBinary = pt.get<std::string>("mainBinary");
        }
        catch(std::runtime_error &e)
        {
            LOGE("unable to interpret module info: "<<e.what());
            return make_error_code(error::general::bad_info_file);
        }

        _state = State::installed;

        return std::error_code{};
    }

    std::error_code Controller::detach()
    {
        switch(_state)
        {
        case State::null:
            return std::error_code{};
            break;

        case State::installed:
        case State::installedCorrupted:
            break;

        default:
            return make_error_code(error::general::module_already_attached);
        }

        if(State::installed != _state)
        {
            return make_error_code(error::general::module_already_attached);
        }

        _provider.clear();
        //_id;
        _serviceIds.clear();

        _revision=0;
        _name.clear();
        _description.clear();
        _tags.clear();

        _requiredServiceIds.clear();
        _requiredModuleIds.clear();

        _state = State::null;
        _place.setDir(std::string{});

        _mainBinary.clear();

        return std::error_code{};
    }

    const std::string &Controller::getProvider() const
    {
        return _provider;
    }

    const Mid &Controller::getId() const
    {
        return _id;
    }

    const std::vector<couple::runtime::Iid> &Controller::getServieceIds() const
    {
        return _serviceIds;
    }

    std::size_t Controller::getRevision() const
    {
        return _revision;
    }

    const std::string &Controller::getName() const
    {
        return _name;
    }

    const std::string &Controller::getDescription() const
    {
        return _description;
    }

    const std::vector<std::string> &Controller::getTags() const
    {
        return _tags;
    }

    const std::vector<couple::runtime::Iid> &Controller::getRequiredServiceIds() const
    {
        return _requiredServiceIds;
    }

    const std::vector<Mid> &Controller::getRequiredModuleIds() const
    {
        return _requiredModuleIds;
    }

    State Controller::getState() const
    {
        return _state;
    }

    async::Future<std::error_code> Controller::install(const Place &place)
    {
        assert(0);
    }

    async::Future<std::error_code> Controller::uninstall()
    {
        assert(0);
    }

    async::Future<std::error_code> Controller::installAfterUpgrade(const Place &place)
    {
        assert(0);
    }

    async::Future<std::error_code> Controller::uninstallBeforeUpgrade()
    {
        assert(0);
    }

    async::Future<std::error_code> Controller::load()
    {
        async::Promise<std::error_code> p;
        async::Future<std::error_code> f = p.future();

        async::spawn([p=std::move(p), this] () mutable {

            if(State::installed != _state)
            {
                p.setValue(make_error_code(error::general::wrong_module_state));
                return;
            }

            boost::filesystem::path mainBinaryPath{_place.getDir()};
            mainBinaryPath /= _mainBinary;

            assert(!_mainBinaryHandle);
            _mainBinaryHandle = dlopen(mainBinaryPath.string().c_str(), RTLD_LAZY|RTLD_LOCAL);

            if(!_mainBinaryHandle)
            {
                LOGE(dlerror());
                p.setValue(make_error_code(error::general::unable_load_binary));
                return;
            }

            assert(!_moduleInstall);
            assert(!_moduleUninstall);

            assert(!_moduleLoad);
            assert(!_moduleUnload);

            assert(!_moduleStart);
            assert(!_moduleStop);

            assert(!_moduleGetServiceInstance);

             _moduleInstall             = (FModuleInstall)              dlsym(_mainBinaryHandle, "lutModuleInstall");
             _moduleUninstall           = (FModuleUninstall)            dlsym(_mainBinaryHandle, "lutModuleUninstall");

             _moduleLoad                = (FModuleLoad)                 dlsym(_mainBinaryHandle, "lutModuleLoad");
             _moduleUnload              = (FModuleUnload)               dlsym(_mainBinaryHandle, "lutModuleUnload");

             _moduleStart               = (FModuleStart)                dlsym(_mainBinaryHandle, "lutModuleStart");
             _moduleStop                = (FModuleStop)                 dlsym(_mainBinaryHandle, "lutModuleStop");

             _moduleGetServiceInstance  = (FModuleGetServiceInstance)   dlsym(_mainBinaryHandle, "lutModuleGetServiceInstance");

             if(_moduleLoad)
             {
                 std::error_code ec = _moduleLoad(himpl::impl2Face<lut::module::Place>(_place)).value<0>();
                 if(ec)
                 {
                     _moduleInstall             = nullptr;
                     _moduleUninstall           = nullptr;

                     _moduleLoad                = nullptr;
                     _moduleUnload              = nullptr;

                     _moduleStart               = nullptr;
                     _moduleStop                = nullptr;

                     _moduleGetServiceInstance  = nullptr;

                     dlclose(_mainBinaryHandle);
                     _mainBinaryHandle = nullptr;

                     LOGE("module \""<<_name<<"\" loading: "<<ec);
                     p.setValue(std::move(ec));
                     return;
                 }
             }

            _state = State::loaded;
            p.setValue(std::error_code{});
        });

        return f;
    }

    async::Future<std::error_code> Controller::unload()
    {
        async::Promise<std::error_code> p;
        async::Future<std::error_code> f = p.future();

        async::spawn([p=std::move(p), this] () mutable {

            if(State::loaded != _state)
            {
                p.setValue(make_error_code(error::general::wrong_module_state));
                return;
            }

            if(_moduleUnload)
            {
                std::error_code ec = _moduleUnload(himpl::impl2Face<lut::module::Place>(_place)).value<0>();
                if(ec)
                {
                    LOGE("module \""<<_name<<"\" unloading: "<<ec);
                    p.setValue(std::move(ec));
                    return;
                }
            }

            _moduleInstall             = nullptr;
            _moduleUninstall           = nullptr;

            _moduleLoad                = nullptr;
            _moduleUnload              = nullptr;

            _moduleStart               = nullptr;
            _moduleStop                = nullptr;

            _moduleGetServiceInstance  = nullptr;


            assert(_mainBinaryHandle);
            dlclose(_mainBinaryHandle);
            _mainBinaryHandle = nullptr;

            _state = State::installed;

            p.setValue(std::error_code{});
            return;
        });

        return f;
    }

    async::Future<std::error_code> Controller::start()
    {
        async::Promise<std::error_code> p;
        async::Future<std::error_code> f = p.future();

        async::spawn([p=std::move(p), this] () mutable {

            if(State::loaded != _state)
            {
                p.setValue(make_error_code(error::general::wrong_module_state));
                return;
            }

            if(_moduleStart)
            {
                std::error_code ec = _moduleStart(himpl::impl2Face<lut::module::Place>(_place)).value<0>();
                if(ec)
                {
                    LOGE("module \""<<_name<<"\" starting: "<<ec);
                    p.setValue(std::move(ec));
                    return;
                }
            }

            _state = State::started;

            p.setValue(std::error_code{});
            return;
        });

        return f;
    }

    async::Future<std::error_code> Controller::stop()
    {
        async::Promise<std::error_code> p;
        async::Future<std::error_code> f = p.future();

        async::spawn([p=std::move(p), this] () mutable {

            if(State::started != _state)
            {
                p.setValue(make_error_code(error::general::wrong_module_state));
                return;
            }

            if(_moduleStop)
            {
                std::error_code ec = _moduleStop(himpl::impl2Face<lut::module::Place>(_place)).value<0>();
                if(ec)
                {
                    LOGE("module \""<<_name<<"\" stopping: "<<ec);
                    p.setValue(std::move(ec));
                    return;
                }
            }

            _state = State::loaded;

            p.setValue(std::error_code{});
            return;
        });

        return f;
    }

    async::Future<std::error_code, couple::runtime::IfacePtr> Controller::getServiceInstance(const couple::runtime::Iid &iid)
    {
        assert(0);
    }

}}}
