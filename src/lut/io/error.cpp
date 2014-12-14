#include "lut/stable.hpp"
#include "lut/io/error.hpp"

namespace lut { namespace io { namespace error
{
    namespace
    {
        const class GeneralCatogory
            : public std::error_category
        {
        public:
            const char* name() const noexcept
            {
                return "lut.io.general";
            }

            std::string message(int value) const
            {
                switch(static_cast<general>(value))
                {
                case general::already_runned:
                    return "already runned";
                case general::not_runned:
                    return "not runned";
                case general::invalid_argument:
                    return "invalid argument";
                }

                return "lut.io.general error";
            }
        } generalCatogory;

        const class StreamCatogory
            : public std::error_category
        {
        public:
            const char* name() const noexcept
            {
                return "lut.io.stream";
            }

            std::string message(int value) const
            {
                switch(static_cast<stream>(value))
                {
                case stream::not_connected:
                    return "not connected";
                case stream::closed:
                    return "closed";
                }

                return "lut.io.stream error";
            }
        } streamCatogory;
    }

    const std::error_category& general_category()
    {
        return generalCatogory;
    }

    const std::error_category& stream_category()
    {
        return streamCatogory;
    }

}}}

namespace lut { namespace io
{
    std::error_code make_error_code(error::general e)
    {
        return std::error_code(static_cast<int>(e), error::general_category());
    }

    std::error_code make_error_code(error::stream e)
    {
        return std::error_code(static_cast<int>(e), error::stream_category());
    }
}}
