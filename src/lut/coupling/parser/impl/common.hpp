#pragma once

#include "lut/coupling/parser/impl/tokens.hpp"

#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/support_line_pos_iterator.hpp>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

namespace lut { namespace coupling { namespace parser { namespace impl
{
    using CharIterator = boost::spirit::line_pos_iterator<boost::spirit::multi_pass<std::istreambuf_iterator<char>>>;
    using Toks = lut::coupling::parser::impl::Tokens<CharIterator>;
    using TokIterator = Toks::iterator_type;
    using Token = Toks::Token;

    Toks toks;

    struct deref_impl
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

    boost::phoenix::function<deref_impl> deref;
}}}}
