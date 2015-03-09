#include "lut/stable.hpp"
#include "lut/couple/parser/errorInfo.hpp"

namespace lut { namespace couple { namespace parser
{

    ErrorInfo::ErrorInfo(const std::string &file, int line, int column, const std::string &message)
        : _file{file}
        , _line{line}
        , _column{column}
        , _message{message}
    {

    }

    std::string ErrorInfo::toString() const
    {
        std::string res;
        if(!_file.empty())
        {
            res += _file;

            if(_line>=0)
            {
                res += ":";
                res += std::to_string(_line);

                if(_column>=0)
                {
                    res += ":";
                    res += std::to_string(_column);
                }
            }

            res += ": ";
        }

        if(!_message.empty())
        {
            res += _message;
        }

        return res;
    }

}}}
