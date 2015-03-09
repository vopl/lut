#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkSet()
    {
        ////////////////////////////////////////////////////////////////////////////////
        set =
            toks.kwset[qi::_val = phx::construct<Set>(phx::new_<SSet>())] >>
            (toks.oab | error(+"'<' expected")) >>
            (
                typeUse[phx::bind(&SSet::elementType, deref(qi::_val)) = qi::_1] |
                error(+"set element type expected")
            ) >>
            (toks.cab | error(+"'>' expected"));
    }

}}}}
