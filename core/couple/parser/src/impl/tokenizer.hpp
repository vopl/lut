#pragma once

#include "common.hpp"
#include "posIterator.hpp"

namespace dci { namespace couple { namespace parser { namespace impl
{

    using CharIterator = PosIterator<boost::spirit::multi_pass<std::istreambuf_iterator<char>>>;

    using Token = boost::spirit::lex::lexertl::token<CharIterator, boost::mpl::vector0<>, boost::mpl::false_>;

    struct Tokenizer
        : boost::spirit::lex::lexer<boost::spirit::lex::lexertl::actor_lexer<Token>>
    {
        Tokenizer();

        using TokenDef = boost::spirit::lex::token_def<>;

        TokenDef ob, cb;        // ()
        TokenDef ocb, ccb;      // {}
        TokenDef oab, cab;      // <>
        TokenDef semi;          // ;
        TokenDef comma;         // ,
        TokenDef dcolon;        // ::
        TokenDef colon;         // :
        TokenDef eq;            // =

        TokenDef quotedString;

        TokenDef uint;

        TokenDef kwvoid;
        TokenDef kwbool;

        TokenDef kwstring;

        TokenDef kwint8;
        TokenDef kwint16;
        TokenDef kwint32;
        TokenDef kwint64;

        TokenDef kwuint8;
        TokenDef kwuint16;
        TokenDef kwuint32;
        TokenDef kwuint64;

        TokenDef kwreal32;
        TokenDef kwreal64;

        TokenDef kwlist;
        TokenDef kwset;
        TokenDef kwmap;
        TokenDef kwptr;
        TokenDef kwarray;

        TokenDef kwscope;
        TokenDef kwinclude;
        TokenDef kwalias;
        TokenDef kwstruct;
        TokenDef kwvariant;
        TokenDef kwiface;
        TokenDef kwenum;
        TokenDef kwin;
        TokenDef kwout;
        TokenDef kwnowait;

        TokenDef id;            // any identifier but not keyword
    };

    using TokIterator = Tokenizer::iterator_type;
    using TokValue = Token::token_value_type;

    extern Tokenizer toks;

}}}}
