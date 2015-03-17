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
                return "lut.module.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<general>(value))
                {
                case general::unknown:
                    return "unknown";
                }

                return "lut.module.general error";
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
