#include "lut/stable.hpp"
#include "lut/coupling/parser/errorInfo.hpp"

namespace lut { namespace coupling { namespace parser
{

    ErrorInfo::ErrorInfo(const std::string &file, int line, const std::error_code &ec)
        : _file{file}
        , _line{line}
        , _ec{ec}
    {

    }

    std::string ErrorInfo::toString() const
    {
        return _ec.message()+", "+_file;
    }

}}}
