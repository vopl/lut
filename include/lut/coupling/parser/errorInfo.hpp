#pragma once

namespace lut { namespace coupling { namespace parser
{
    class ErrorInfo
    {
    public:
        ErrorInfo(const std::string &file, int line, const std::error_code &ec);

        std::string toString() const;

    private:
        std::string _file;
        int _line;
        std::error_code _ec;
    };
}}}
