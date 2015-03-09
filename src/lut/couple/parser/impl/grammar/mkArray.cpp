#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkArray()
    {
        ////////////////////////////////////////////////////////////////////////////////
        array =
            toks.kwarray[qi::_val = phx::construct<Array>(phx::new_<SArray>())] >>
            (toks.oab | error(+"'<' expected")) >>
            (
                typeUse[phx::bind(&SArray::elementType, deref(qi::_val)) = qi::_1] |
                error(+"array element type expected")
            ) >>
            (toks.comma | error(+"',' expected")) >>
            (
                toks.uint[phx::bind(&SArray::size, deref(qi::_val)) = phx::construct<std::string>(phx::begin(qi::_1), phx::end(qi::_1))] |
                error(+"array size expected")
            ) >>
            (toks.cab | error(+"'>' expected"));
    }

}}}}
