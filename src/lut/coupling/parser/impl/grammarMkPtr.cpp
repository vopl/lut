#include "lut/coupling/parser/impl/grammar.hpp"

namespace lut { namespace coupling { namespace parser { namespace impl
{
    void Grammar::mkPtr()
    {
        ////////////////////////////////////////////////////////////////////////////////
        ptr =
            toks.kwptr[qi::_val = phx::construct<Ptr>(phx::new_<SPtr>())] >>
            (toks.oab | error(+"'<' expected")) >>
            (
                typeUse[phx::bind(&SPtr::valueType, deref(qi::_val)) = qi::_1] |
                error(+"pointer value type expected")
            ) >>
            (toks.cab | error(+"'>' expected"));
    }

}}}}
