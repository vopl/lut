#pragma once

#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/repository/home/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace dci { namespace couple { namespace parser { namespace impl
{
    struct derefEngine
    {
        template<class T> struct result
        {
            typedef T & type;
        };

        template<class T>
        struct result<std::shared_ptr<T>>
        {
            typedef T & type;
        };

        template <typename T>
        T &operator()(T &arg1) const
        {
            return arg1;
        }

        template <typename T>
        T &operator()(std::shared_ptr<T> arg1) const
        {
            return *arg1;
        }
    };

    extern boost::phoenix::function<derefEngine> deref;

}}}}
