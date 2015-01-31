#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkList()
    {
        ////////////////////////////////////////////////////////////////////////////////
        list =
            toks.kwlist[qi::_val = phx::construct<List>(phx::new_<SList>())] >>
            (toks.oab | error(+"'<' expected")) >>
            (
                typeUse[phx::bind(&SList::elementType, deref(qi::_val)) = qi::_1] |
                error(+"list element type expected")
            ) >>
            (toks.cab | error(+"'>' expected"));
    }

}}}}
