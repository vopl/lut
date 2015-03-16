#include "lut/module/controller.hpp"
#include "lut/module/impl/controller.hpp"
#include "lut/himpl/face2Impl.hpp"

namespace lut { namespace module
{
    Controller::Controller()
        : himpl::FaceLayout<impl::Controller>()
    {
    }

    Controller::~Controller()
    {
    }

    std::error_code Controller::attach(const Place &place)
    {
        return impl().attach(himpl::face2Impl(place));
    }

    std::error_code Controller::detach()
    {
        return impl().detach();
    }

    const std::string &Controller::getProvider() const
    {
        return impl().getProvider();
    }

    const Mid &Controller::getId() const
    {
        return impl().getId();
    }

    const std::vector<couple::runtime::Iid> &Controller::getServieceIds() const
    {
        return impl().getServieceIds();
    }

    std::size_t Controller::getRevision() const
    {
        return impl().getRevision();
    }

    const std::string &Controller::getName() const
    {
        return impl().getName();
    }

    const std::string &Controller::getDescription() const
    {
        return impl().getDescription();
    }

    const std::vector<std::string> &Controller::getTags() const
    {
        return impl().getTags();
    }

    const std::vector<couple::runtime::Iid> &Controller::getRequiredServiceIds() const
    {
        return impl().getRequiredServiceIds();
    }

    const std::vector<Mid> &Controller::getRequiredModuleIds() const
    {
        return impl().getRequiredModuleIds();
    }

    State Controller::getState() const
    {
        return impl().getState();
    }

    async::Future<std::error_code> Controller::install(const Place &place)
    {
        return impl().install(himpl::face2Impl(place));
    }

    async::Future<std::error_code> Controller::uninstall()
    {
        return impl().uninstall();
    }

    async::Future<std::error_code> Controller::installAfterUpgrade(const Place &place)
    {
        return impl().installAfterUpgrade(himpl::face2Impl(place));
    }

    async::Future<std::error_code> Controller::uninstallBeforeUpgrade()
    {
        return impl().uninstallBeforeUpgrade();
    }

    async::Future<std::error_code> Controller::load()
    {
        return impl().load();
    }

    async::Future<std::error_code> Controller::unload()
    {
        return impl().unload();
    }

    async::Future<std::error_code> Controller::start()
    {
        return impl().start();
    }

    async::Future<std::error_code> Controller::stop()
    {
        return impl().stop();
    }

    async::Future<std::error_code, couple::runtime::IfacePtr> Controller::getServiceInstance(const couple::runtime::Iid &iid)
    {
        return impl().getServiceInstance(iid);
    }

}}
