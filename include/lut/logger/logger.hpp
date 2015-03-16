#pragma once

#include <iostream>
#include <system_error>
#include <boost/system/error_code.hpp>

namespace lut { namespace logger
{
    struct LogStream
    {
        template <class T> LogStream &operator<<(const T& v)
        {
            std::cout<<v;
            return *this;
        }

        LogStream &operator<<( std::ostream&(*v)(std::ostream&) )
        {
            std::cout<<v;
            return *this;
        }
    };

    inline LogStream& operator<< (LogStream& os, const std::error_code &ec)
    {
        os << ec.message() << " (" << ec.category().name() << '.' << ec.value() << ')';
        return os;
    }

    inline LogStream& operator<< (LogStream& os, const boost::system::error_code &ec)
    {
        os << ec.message() << " (" << ec.category().name() << '.' << ec.value() << ')';
        return os;
    }
}}

#   define LOGF(msg) ::lut::logger::LogStream()<<msg<<::std::endl;
#   define LOGE(msg) ::lut::logger::LogStream()<<msg<<::std::endl;
#   define LOGW(msg) ::lut::logger::LogStream()<<msg<<::std::endl;
#   define LOGI(msg) ::lut::logger::LogStream()<<msg<<::std::endl;
#   define LOGD(msg) ::lut::logger::LogStream()<<msg<<::std::endl;
#   define LOGT(msg) ::lut::logger::LogStream()<<msg<<::std::endl;

