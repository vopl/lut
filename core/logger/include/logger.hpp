#pragma once

#include <iostream>
#include <system_error>
#include <boost/system/error_code.hpp>

namespace dci { namespace logger
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

#   define LOGF(msg) ::dci::logger::LogStream()<<msg<<::std::endl;
#   define LOGE(msg) ::dci::logger::LogStream()<<msg<<::std::endl;
#   define LOGW(msg) ::dci::logger::LogStream()<<msg<<::std::endl;
#   define LOGI(msg) ::dci::logger::LogStream()<<msg<<::std::endl;
#   define LOGD(msg) ::dci::logger::LogStream()<<msg<<::std::endl;
#   define LOGT(msg) ::dci::logger::LogStream()<<msg<<::std::endl;

