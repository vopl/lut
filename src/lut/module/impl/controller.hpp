#include "lut/himpl/implLayout.hpp"

#include "lut/module/place.hpp"
#include "lut/couple/runtime/identifier.hpp"
#include "lut/couple/runtime/iface.hpp"
#include "lut/module/mid.hpp"
#include "lut/module/state.hpp"

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
        Controller(const Place &place);
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

    };
}}}
