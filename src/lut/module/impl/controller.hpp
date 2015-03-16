#include "lut/himpl/implLayout.hpp"

#include "lut/module/place.hpp"
#include "lut/couple/runtime/identifier.hpp"
#include "lut/couple/runtime/iface.hpp"
#include "lut/module/mid.hpp"
#include "lut/module/state.hpp"
#include "lut/module/impl/place.hpp"

#include "lut/async/future.hpp"

#include <string>
#include <vector>
#include <system_error>
#include <memory>

namespace lut { namespace module { namespace impl
{
    class Controller
        : public himpl::ImplLayout<Controller>
    {
    public:
        Controller();
        ~Controller();

        std::error_code attach(const Place &place);
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

        async::Future<std::error_code> install(const Place &place);
        async::Future<std::error_code> uninstall();

        async::Future<std::error_code> installAfterUpgrade(const Place &place);
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
        Place                               _place;


    private:
        void *  _mainBinaryHandle;

        using FModuleInstall            = async::Future<std::error_code> (*)(const lut::module::Place &place);
        using FModuleUninstall          = async::Future<std::error_code> (*)(const lut::module::Place &place);

        using FModuleLoad               = async::Future<std::error_code> (*)(const lut::module::Place &place);
        using FModuleUnload             = async::Future<std::error_code> (*)(const lut::module::Place &place);

        using FModuleStart              = async::Future<std::error_code> (*)(const lut::module::Place &place);
        using FModuleStop               = async::Future<std::error_code> (*)(const lut::module::Place &place);

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
}}}
