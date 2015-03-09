#include "lut/couple/parser/impl/grammar.hpp"

namespace lut { namespace couple { namespace parser { namespace impl
{
    void Grammar::mkMap()
    {
        ////////////////////////////////////////////////////////////////////////////////
        map =
            toks.kwmap[qi::_val = phx::construct<Map>(phx::new_<SMap>())] >>
            (toks.oab | error(+"'<' expected")) >>
            (
                typeUse[phx::bind(&SMap::keyType, deref(qi::_val)) = qi::_1] |
                error(+"map key type expected")
            ) >>
            (toks.comma | error(+"',' expected")) >>
            (
                typeUse[phx::bind(&SMap::valueType, deref(qi::_val)) = qi::_1] |
                error(+"map value type expected")
            ) >>
            (toks.cab | error(+"'>' expected"));
    }

}}}}
