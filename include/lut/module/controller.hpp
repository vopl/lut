#pragma once

#include "lut/himpl/faceLayout.hpp"
#include "lut/module/sizeProvider.hpp"

#include "lut/module/place.hpp"
#include "lut/couple/runtime/identifier.hpp"
#include "lut/couple/runtime/iface.hpp"

#include "lut/async/future.hpp"

#include <string>
#include <vector>
#include <system_error>
#include <memory>

namespace lut { namespace module
{
    struct ModuleIdentifierTag;
    using Mid = couple::runtime::Identifier<ModuleIdentifierTag>;

    enum class State
    {
        unknown,
        installed,
        loaded,
        started,
    };

    namespace impl
    {
        class Controller;
    }

    class Controller
        : public himpl::FaceLayout<impl::Controller>
    {
        Controller(const Controller &) = delete;
        void operator=(const Controller &) = delete;

    public:
        Controller(...);//from some identification info
        ~Controller();

        ////////////// identify
        const std::string &getProvider() const;

        Mid getId() const;
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
        async::Future<std::error_code> uninstall(const Place &place);

        async::Future<std::error_code> installAfterUpgrade(const Place &place);
        async::Future<std::error_code> uninstallBeforeUpgrade(const Place &place);

        async::Future<std::error_code> load(const Place &place);
        async::Future<std::error_code> unload();


        /////////////// run
        async::Future<std::error_code> start();
        async::Future<std::error_code> stop();




        //////////////// use
        async::Future<std::error_code, couple::runtime::IfacePtr> getServiceInstance(const couple::runtime::Iid &iid);
    };

    using ControllerPtr = std::unique_ptr<Controller>;
}}
