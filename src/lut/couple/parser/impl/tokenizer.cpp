#include "lut/couple/parser/impl/tokenizer.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    Tokenizer::Tokenizer()
        : ob        {'('}
        , cb        {')'}
        , ocb       {'{'}
        , ccb       {'}'}
        , oab       {'<'}
        , cab       {'>'}
        , semi      {';'}
        , comma     {','}
        , dcolon    {"\"::\""}
        , colon     {':'}
        , eq        {'='}

        , quotedString {"\\\"((\\\\\\\")|[^\\\"])*\\\""}
        , uint      {"\\d+"}

        , kwvoid    {"\"void\""}
        , kwbool    {"\"bool\""}
        , kwstring  {"\"string\""}
        , kwint8    {"\"int8\""}
        , kwint16   {"\"int16\""}
        , kwint32   {"\"int32\""}
        , kwint64   {"\"int64\""}
        , kwuint8   {"\"uint8\""}
        , kwuint16  {"\"uint16\""}
        , kwuint32  {"\"uint32\""}
        , kwuint64  {"\"uint64\""}
        , kwreal32  {"\"real32\""}
        , kwreal64  {"\"real64\""}

        , kwlist    {"\"list\""}
        , kwset     {"\"set\""}
        , kwmap     {"\"map\""}
        , kwptr     {"\"ptr\""}
        , kwarray   {"\"array\""}

        , kwscope   {"\"scope\""}
        , kwinclude {"\"include\""}
        , kwalias   {"\"alias\""}
        , kwstruct  {"\"struct\""}
        , kwvariant {"\"variant\""}
        , kwiface   {"\"iface\""}
        , kwenum    {"\"enum\""}
        , kwin      {"\"in\""}
        , kwout     {"\"out\""}
        , kwnowait  {"\"nowait\""}

        , id        {"[a-zA-Z_]+\\w*"}
    {
        this->self
            = TokenDef{"\\s"}                       [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]
            | TokenDef{"\\/\\/[^\\r\\n]*\\r?\\n?"}  [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]
            | TokenDef{"\\/\\*(.*?)\\*\\/"}         [boost::spirit::lex::_pass = boost::spirit::lex::pass_flags::pass_ignore]

            | ob
            | cb
            | ocb
            | ccb
            | oab
            | cab
            | semi
            | comma
            | dcolon
            | colon
            | eq

            | quotedString

            | uint

            | kwvoid
            | kwbool
            | kwstring
            | kwint8
            | kwint16
            | kwint32
            | kwint64
            | kwuint8
            | kwuint16
            | kwuint32
            | kwuint64
            | kwreal32
            | kwreal64

            | kwlist
            | kwset
            | kwmap
            | kwptr
            | kwarray

            | kwscope
            | kwinclude
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

    Tokenizer toks;

}}}}
