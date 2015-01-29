#pragma once

#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_lazy.hpp>

namespace lut { namespace coupling { namespace parser { namespace impl
{

    template <typename TIterator>
    struct Tokens
        : boost::spirit::lex::lexer<boost::spirit::lex::lexertl::actor_lexer<boost::spirit::lex::lexertl::token<TIterator, boost::mpl::vector0<>, boost::mpl::false_>>>
    {
        using Token = typename Tokens::token_type;

        Tokens()
            : ob        {'('}
            , cb        {')'}
            , ocb       {'{'}
            , ccb       {'}'}
            , semi      {';'}
            , comma     {','}
            , colon     {':'}
            , eq        {'='}

            , kwscope   {"\"scope\""}
            , kwimport  {"\"import\""}
            , kwalias   {"\"alias\""}
            , kwstruct  {"\"struct\""}
            , kwvariant {"\"variant\""}
            , kwiface   {"\"iface\""}
            , kwenum    {"\"enum\""}
            , kwin      {"\"in\""}
            , kwout     {"\"out\""}
            , kwnowait  {"\"nowait\""}

            , id        {"[a-zA-Z_]+\\w*"}

            , word      {kwscope | kwimport | kwalias | kwstruct | kwvariant | kwiface | kwenum | kwin | kwout | kwnowait | id}
        {
            this->self
                = TokenDef{"\\s"}                       [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]
                | TokenDef{"\\/\\/[^\\r\\n]*\\r?\\n?"}  [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]
                | TokenDef{"\\/\\*(.*?)\\*\\/"}         [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]

                | ob
                | cb
                | ocb
                | ccb
                | semi
                | comma
                | colon
                | eq

                | kwscope
                | kwimport
                | kwalias
                | kwstruct
                | kwvariant
                | kwiface
                | kwenum
                | kwin
                | kwout
                | kwnowait

                | id

            ;
        }

        using TokenDef = boost::spirit::lex::token_def<>;

        TokenDef ob, cb;        // ()
        TokenDef ocb, ccb;      // {}
        TokenDef semi;          // ;
        TokenDef comma;         // ,
        TokenDef colon;         // :
        TokenDef eq;            // =

        TokenDef kwscope;
        TokenDef kwimport;
        TokenDef kwalias;
        TokenDef kwstruct;
        TokenDef kwvariant;
        TokenDef kwiface;
        TokenDef kwenum;
        TokenDef kwin;
        TokenDef kwout;
        TokenDef kwnowait;

        TokenDef id;            // any identifier but not keyword

        boost::spirit::qi::rule<typename Tokens::iterator_type, typename Token::token_value_type ()> word; // any identifier or keyword
    };

}}}}
