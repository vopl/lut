#include "lut/stable.hpp"
#include "lut/module/error.hpp"

namespace lut { namespace module { namespace error
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
                case general::module_already_attached:
                    return "module already attached";
                case general::module_busy:
                    return "module busy";
                case general::wrong_module_state:
                    return "wrong module state";
                case general::unable_load_binary:
                    return "unable to load binary";
                case general::malformed_module_binary:
                    return "malformed module binary";
                case general::bad_info_file:
                    return "bad info file";
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

namespace lut { namespace module
{
    std::error_code make_error_code(error::general e)
    {
        return std::error_code(static_cast<int>(e), error::general_category());
    }
}}
