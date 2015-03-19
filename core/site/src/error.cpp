#include <error.hpp>

namespace dci { namespace site { namespace error
{
    namespace
    {
        const class GeneralCatogory
            : public std::error_category
        {
        public:
            const char* name() const noexcept
            {
                return "dci.site.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<general>(value))
                {
                case general::unknown:
                    return "unknown";
                case general::already_started:
                    return "already started";
                case general::already_stopped:
                    return "already stopped";
                case general::starting_in_progress:
                    return "starting in progress";
                case general::stopping_in_progress:
                    return "stopping in progress";
                case general::modules_directory_absent:
                    return "modules directory is absent";
                case general::failed:
                    return "failed";
                case general::parial_failed:
                    return "parial failed";
                }

                return "dci.site.general error";
            }
        } generalCatogory;


        const class ModuleCatogory
            : public std::error_category
        {
        public:
            const char* name() const noexcept
            {
                return "dci.site.module";
            }

            std::string message(int value) const
            {
                switch(static_cast<module>(value))
                {
                case module::unknown:
                    return "unknown";
                case module::busy:
                    return "busy";
                case module::wrong_state:
                    return "wrong state";
                case module::unable_load_binary:
                    return "unable to load binary";
                case module::malformed_binary:
                    return "malformed binary";
                case module::bad_info_file:
                    return "bad info file";
                }

                return "dci.site.module error";
            }
        } moduleCatogory;

    }

    const std::error_category& general_category()
    {
        return generalCatogory;
    }

    const std::error_category& module_category()
    {
        return moduleCatogory;
    }

}}}

namespace dci { namespace site
{
    std::error_code make_error_code(error::general e)
    {
        return std::error_code(static_cast<int>(e), error::general_category());
    }

    std::error_code make_error_code(error::module e)
    {
        return std::error_code(static_cast<int>(e), error::module_category());
    }
}}
