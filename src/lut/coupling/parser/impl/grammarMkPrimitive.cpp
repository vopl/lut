#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkPrimitive()
    {
        ////////////////////////////////////////////////////////////////////////////////
        primitive =
            toks.kwvoid     [qi::_val = Primitive::void_    ] |
            toks.kwbool     [qi::_val = Primitive::bool_    ] |
            toks.kwstring   [qi::_val = Primitive::string   ] |
            toks.kwint8     [qi::_val = Primitive::int8     ] |
            toks.kwint16    [qi::_val = Primitive::int16    ] |
            toks.kwint32    [qi::_val = Primitive::int32    ] |
            toks.kwint64    [qi::_val = Primitive::int64    ] |
            toks.kwuint8    [qi::_val = Primitive::uint8    ] |
            toks.kwuint16   [qi::_val = Primitive::uint16   ] |
            toks.kwuint32   [qi::_val = Primitive::uint32   ] |
            toks.kwuint64   [qi::_val = Primitive::uint64   ] |
            toks.kwreal32   [qi::_val = Primitive::real32   ] |
            toks.kwreal64   [qi::_val = Primitive::real64   ];

    }

}}}}
