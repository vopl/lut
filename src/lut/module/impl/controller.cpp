#include "lut/module/impl/controller.hpp"

namespace lut { namespace module { namespace impl
{
    Controller::Controller()
        : himpl::ImplLayout<Controller>{}
    {
        assert(0);
    }

    Controller::Controller(const Place &place)
        : himpl::ImplLayout<Controller>{}
    {
        assert(0);
    }

    Controller::~Controller()
    {
        assert(0);
    }

    const std::string &Controller::getProvider() const
    {
        assert(0);
    }

    Mid Controller::getId() const
    {
        assert(0);
    }

    const std::vector<couple::runtime::Iid> &Controller::getServieceIds() const
    {
        assert(0);
    }

    std::size_t Controller::getRevision() const
    {
        assert(0);
    }

    const std::string &Controller::getName() const
    {
        assert(0);
    }

    const std::string &Controller::getDescription() const
    {
        assert(0);
    }

    const std::vector<std::string> &Controller::getTags() const
    {
        assert(0);
    }

    const std::vector<couple::runtime::Iid> &Controller::getRequiredServiceIds() const
    {
        assert(0);
    }

    const std::vector<Mid> &Controller::getRequiredModuleIds() const
    {
        assert(0);
    }

    State Controller::getState() const
    {
        assert(0);
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
        assert(0);
    }

    async::Future<std::error_code> Controller::unload()
    {
        assert(0);
    }

    async::Future<std::error_code> Controller::start()
    {
        assert(0);
    }

    async::Future<std::error_code> Controller::stop()
    {
        assert(0);
    }

    async::Future<std::error_code, couple::runtime::IfacePtr> Controller::getServiceInstance(const couple::runtime::Iid &iid)
    {
        assert(0);
    }

}}}
