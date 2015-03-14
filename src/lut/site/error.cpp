#include "lut/stable.hpp"
#include "lut/site/error.hpp"

namespace lut { namespace site { namespace error
{
    namespace
    {
        const class GeneralCatogory
            : public std::error_category
        {
        public:
            const char* name() const noexcept
            {
                return "lut.site.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<general>(value))
                {
                case general::unknown:
                    return "unknown";
                case general::already_runned:
                    return "already runned";
                case general::stopping_in_progress:
                    return "stopping in progress";
                case general::modules_directory_absent:
                    return "modules directory is absent";
                case general::failed:
                    return "failed";
                }

                return "lut.site.general error";
            }
        } generalCatogory;
    }

    const std::error_category& general_category()
    {
        return generalCatogory;
    }

}}}

namespace lut { namespace site
{
    std::error_code make_error_code(error::general e)
    {
        return std::error_code(static_cast<int>(e), error::general_category());
    }
}}
