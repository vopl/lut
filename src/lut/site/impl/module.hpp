#include "lut/himpl/implLayout.hpp"

#include "lut/couple/runtime/identifier.hpp"
#include "lut/couple/runtime/iface.hpp"
#include "lut/site/moduleId.hpp"
#include "lut/site/impl/moduleState.hpp"
#include "lut/site/modulePlace.hpp"
#include "lut/site/impl/modulePlace.hpp"
#include "lut/site/impl/moduleState.hpp"

#include "lut/async/future.hpp"

#include <string>
#include <vector>
#include <system_error>
#include <memory>

namespace lut { namespace site { namespace impl
{
    class Module
        : public himpl::ImplLayout<Module>
    {
    public:
        Module();
        ~Module();

        std::error_code attach(const ModulePlace &place);
        std::error_code detach();

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

        /////////////// install
        State getState() const;

        async::Future<std::error_code> install(const ModulePlace &place);
        async::Future<std::error_code> uninstall();

        async::Future<std::error_code> installAfterUpgrade(const ModulePlace &place);
        async::Future<std::error_code> uninstallBeforeUpgrade();

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

        State                               _state;
        ModulePlace                         _place;


    private:
        void *  _mainBinaryHandle;

        using FModuleInstall            = async::Future<std::error_code> (*)(const lut::site::ModulePlace &place);
        using FModuleUninstall          = async::Future<std::error_code> (*)(const lut::site::ModulePlace &place);

        using FModuleLoad               = async::Future<std::error_code> (*)(const lut::site::ModulePlace &place);
        using FModuleUnload             = async::Future<std::error_code> (*)(const lut::site::ModulePlace &place);

        using FModuleStart              = async::Future<std::error_code> (*)(const lut::site::ModulePlace &place);
        using FModuleStop               = async::Future<std::error_code> (*)(const lut::site::ModulePlace &place);

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
