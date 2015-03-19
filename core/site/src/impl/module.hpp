#include <dci/himpl/implLayout.hpp>

#include <dci/couple/runtime/identifier.hpp>
#include <dci/couple/runtime/iface.hpp>
#include <moduleId.hpp>
#include "moduleState.hpp"
#include <modulePlace.hpp>
#include "modulePlace.hpp"

#include <dci/async/future.hpp>

#include <string>
#include <vector>
#include <system_error>
#include <memory>

namespace dci { namespace site { namespace impl
{
    class Module
        : public himpl::ImplLayout<Module>
    {
    public:
        Module();
        ~Module();

        ////////////// identify
        const std::string &getProvider() const;

        const Mid &getId() const;
        const std::vector<couple::runtime::Iid> &getServieceIds() const;

        std::size_t getRevision() const;

        const std::string &getName() const;
        const std::string &getDescription() const;
        const std::vector<std::string> &getTags() const;

        /////////////////// dependencies
        const std::vector<couple::runtime::Iid> &getRequiredServiceIds() const;
        const std::vector<Mid> &getRequiredModuleIds() const;

        ModuleState getState() const;

        /////////////// attach
        std::error_code attach(const ModulePlace &place);
        std::error_code detach();

        /////////////// install
        async::Future<std::error_code> install(const ModulePlace &place);
        async::Future<std::error_code> uninstall();

        /////////////// load
        async::Future<std::error_code> load();
        async::Future<std::error_code> unload();

        /////////////// run
        async::Future<std::error_code> start();
        async::Future<std::error_code> stop();




        //////////////// use
        async::Future<std::error_code, couple::runtime::IfacePtr> getServiceInstance(const couple::runtime::Iid &iid);

    private:
        std::string                         _provider;
        Mid                                 _id;
        std::vector<couple::runtime::Iid>   _serviceIds;

        std::size_t                         _revision;
        std::string                         _name;
        std::string                         _description;
        std::vector<std::string>            _tags;

        std::vector<couple::runtime::Iid>   _requiredServiceIds;
        std::vector<Mid>                    _requiredModuleIds;

        std::string                         _mainBinary;

        ModuleState                               _state;
        ModulePlace                         _place;


    private:
        void *  _mainBinaryHandle;

        using FModuleInstall            = async::Future<std::error_code> (*)(const dci::site::ModulePlace &place);
        using FModuleUninstall          = async::Future<std::error_code> (*)(const dci::site::ModulePlace &place);

        using FModuleLoad               = async::Future<std::error_code> (*)(const dci::site::ModulePlace &place);
        using FModuleUnload             = async::Future<std::error_code> (*)(const dci::site::ModulePlace &place);

        using FModuleStart              = async::Future<std::error_code> (*)(const dci::site::ModulePlace &place);
        using FModuleStop               = async::Future<std::error_code> (*)(const dci::site::ModulePlace &place);

        using FModuleGetServiceInstance = async::Future<std::error_code, couple::runtime::IfacePtr> (*)(const couple::runtime::Iid &iid);



    private:
        FModuleInstall              _moduleInstall;
        FModuleUninstall            _moduleUninstall;

        FModuleLoad                 _moduleLoad;
        FModuleUnload               _moduleUnload;

        FModuleStart                _moduleStart;
        FModuleStop                 _moduleStop;

        FModuleGetServiceInstance   _moduleGetServiceInstance;

    };

    using ModulePtr = std::unique_ptr<Module>;
}}}
